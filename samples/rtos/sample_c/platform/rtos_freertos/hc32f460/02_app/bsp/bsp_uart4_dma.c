#include "bsp_uart4_dma.h"
#include "bsp_uart_dma.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uav_logger.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USING_UART4

#define UART4_TX_BUF_LEN     (1024UL)
#define UART4_RX_BUF_LEN     (1024UL)
TxDmaBufDef(4, UART4_TX_BUF_LEN)
RxDmaBufDef(4, UART4_RX_BUF_LEN)
TxRingBufDef(4, 2048)
RxRingBufDef(4, 2048*10)
BspDevDef(4)
ReadWriteVerDef(4)

static void uart4_ringbuff_init(void)
{
    uart4_dev.tx_ring.buffer = uart4_tx_ring_buf;
    uart4_dev.rx_ring.buffer = uart4_rx_ring_buf;
    RingBuffer_create(&uart4_dev.tx_ring, sizeof(uart4_tx_ring_buf));
    RingBuffer_create(&uart4_dev.rx_ring, sizeof(uart4_rx_ring_buf));
    
}

void uart4_dma_tx(void *buf, uint16_t len)
{
    uint16_t remain = len;
    if(enTx4FrameEnd == true)
    {
        remain = (remain <= UART4_TX_BUF_LEN) ? 0: (remain - UART4_TX_BUF_LEN);
        memcpy(u8Tx4DmaBuf, buf, (len - remain));

        DMA_SetSrcAddr(TX4_DMA_UNIT, TX4_DMA_CH, (uint32_t)u8Tx4DmaBuf);
        DMA_SetTransCount(TX4_DMA_UNIT, TX4_DMA_CH, (len - remain));
        (void)DMA_ChCmd(TX4_DMA_UNIT, TX4_DMA_CH, ENABLE);
        USART_FuncCmd(UART4_UNIT, USART_TX, ENABLE);
        enTx4FrameEnd = false; 
    
    }    

    if(remain > 0)
    {
        RingBuffer_write(&uart4_dev.tx_ring, (const uint8_t *)(buf + (len - remain)), remain);
    }
}

uint16_t uart4_read(uint8_t *buf, uint16_t len)
{
	uint16_t real_len = 0;
    real_len = RingBuffer_read(&uart4_dev.rx_ring, buf, len);
	// if(real_len) UAV_LOG_INFO("uart3_read: %d, %s", real_len, buf);
	return real_len;
}

void uart4_set_baudrate(uint32_t baud)
{
    float32_t fErr= 0;
    USART_SetBaudrate(UART4_UNIT, baud, &fErr);
}
static void uart4_dma_tx_from_fifo(uint16_t len)
{
	uint16_t bytes = RingBuffer_read(&uart4_dev.tx_ring, u8Tx4DmaBuf, len);

	DMA_SetSrcAddr(TX4_DMA_UNIT, TX4_DMA_CH, (uint32_t)u8Tx4DmaBuf);
	DMA_SetTransCount(TX4_DMA_UNIT, TX4_DMA_CH, bytes);
	(void)DMA_ChCmd(TX4_DMA_UNIT, TX4_DMA_CH, ENABLE);
	USART_FuncCmd(UART4_UNIT, USART_TX, ENABLE);
	enTx4FrameEnd= RESET;
}

static void UART4_RxDMA_TC_IrqCallback(void)
{
    USART_FuncCmd(UART4_UNIT, USART_RX_TIMEOUT, DISABLE);
    DMA_ClearTransCompleteStatus(RX4_DMA_UNIT, RX4_DMA_TC_FLAG);
}

static void TX4_DMA_TC_IrqCallback(void)
{	
	USART_FuncCmd(UART4_UNIT, USART_INT_TX_CPLT, ENABLE);
	DMA_ClearTransCompleteStatus(TX4_DMA_UNIT, TX4_DMA_TC_FLAG);
}

static void UART4_RxTimeout_IrqCallback(void)
{
	TMR0_Stop(RX4_TMR0_UNIT, RX4_TMR0_CH);
	
	uint16_t i= 0, len= 0;
	
	i= (uint16_t)DMA_GetDestRepeatSize(RX4_DMA_UNIT, RX4_DMA_CH);//
	
	Rx4DmaRear = (UART4_RX_BUF_LEN- i) % UART4_RX_BUF_LEN;
	if(Rx4DmaRear > Rx4DmaFront)
	{
		len= Rx4DmaRear - Rx4DmaFront;
		RingBuffer_write(&uart4_dev.rx_ring, &u8Rx4DmaBuf[Rx4DmaFront], len);
		Rx4DmaFront = (Rx4DmaFront + len) % UART4_RX_BUF_LEN;
		// UAV_LOG_INFO("len: %d", len);	
	}
	else if(Rx4DmaRear < Rx4DmaFront)
	{
		len= UART4_RX_BUF_LEN - Rx4DmaFront;
		RingBuffer_write(&uart4_dev.rx_ring, &u8Rx4DmaBuf[Rx4DmaFront], len);
		Rx4DmaFront= 0;
		
		len= Rx4DmaRear - Rx4DmaFront;
		RingBuffer_write(&uart4_dev.rx_ring, &u8Rx4DmaBuf[Rx4DmaFront], len);
		Rx4DmaFront = (Rx4DmaFront + len) % UART4_RX_BUF_LEN;
		// UAV_LOG_INFO("len: %d", len);	
	}
	else
	{
		USART_ReadData(UART4_UNIT);
		USART_ClearStatus(UART4_UNIT, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR | USART_FLAG_OVERRUN));
	}
	USART_ClearStatus(UART4_UNIT, USART_FLAG_RX_TIMEOUT);
}

static void UART4_TxComplete_IrqCallback(void)
{
	USART_FuncCmd(UART4_UNIT, (USART_TX | USART_INT_TX_CPLT), DISABLE);

	int available_data = RingBuffer_available_data(&uart4_dev.tx_ring);
	if(available_data <= 0)
	{
		USART_ClearStatus(UART4_UNIT, USART_FLAG_TX_CPLT);
		enTx4FrameEnd= SET;
		return;
	}

	int len= (available_data >= UART4_TX_BUF_LEN)? UART4_TX_BUF_LEN : available_data;
	uart4_dma_tx_from_fifo(len);

	USART_ClearStatus(UART4_UNIT, USART_FLAG_TX_CPLT);
}

static void UART4_RxError_IrqCallback(void)
{
	(void)USART_ReadData(UART4_UNIT);

	USART_ClearStatus(UART4_UNIT, (USART_FLAG_PARITY_ERR | USART_FLAG_FRAME_ERR | USART_FLAG_OVERRUN));
}

static void uart_rx_timout_config(CM_USART_TypeDef *USARTx, uint16_t u16TimeoutBits)
{
    uint16_t u16Div;
    uint16_t u16Delay;
    uint16_t u16CompareValue;
    stc_tmr0_init_t stcTmr0Init;
	CM_TMR0_TypeDef *TMR0x=NULL; 
	uint32_t u32Ch= 0;

	if (USARTx == CM_USART1)
	{
		TMR0x= RX1_TMR0_UNIT;
		u32Ch= RX1_TMR0_CH;
		RX1_TMR0_FCG_ENABLE();
	}
	else if (USARTx == CM_USART2)
	{
		TMR0x= RX2_TMR0_UNIT;
		u32Ch= RX2_TMR0_CH;
		RX2_TMR0_FCG_ENABLE();
	}
	else if (USARTx == CM_USART3)
	{
		TMR0x= RX3_TMR0_UNIT;
		u32Ch= RX3_TMR0_CH;
		RX3_TMR0_FCG_ENABLE();
	}
	else if (USARTx == CM_USART4)
	{
		TMR0x= RX4_TMR0_UNIT;
		u32Ch= RX4_TMR0_CH;
		RX4_TMR0_FCG_ENABLE();
	}
	else
	{
		return;
	}

    /* Initialize TMR0 base function. */
    stcTmr0Init.u32ClockSrc = TMR0_CLK_SRC_XTAL32;
    stcTmr0Init.u32ClockDiv = TMR0_CLK_DIV1;
    stcTmr0Init.u32Func     = TMR0_FUNC_CMP;
    if (TMR0_CLK_DIV1 == stcTmr0Init.u32ClockDiv) {
        u16Delay = 7U;
    } else if (TMR0_CLK_DIV2 == stcTmr0Init.u32ClockDiv) {
        u16Delay = 5U;
    } else if ((TMR0_CLK_DIV4 == stcTmr0Init.u32ClockDiv) || \
               (TMR0_CLK_DIV8 == stcTmr0Init.u32ClockDiv) || \
               (TMR0_CLK_DIV16 == stcTmr0Init.u32ClockDiv)) {
        u16Delay = 3U;
    } else {
        u16Delay = 2U;
    }

    u16Div = (uint16_t)1U << (stcTmr0Init.u32ClockDiv >> TMR0_BCONR_CKDIVA_POS);
    u16CompareValue = ((u16TimeoutBits + u16Div - 1U) / u16Div) - u16Delay;
    stcTmr0Init.u16CompareValue = u16CompareValue;
	
    (void)TMR0_Init(TMR0x, u32Ch, &stcTmr0Init);

    TMR0_HWStartCondCmd(TMR0x, u32Ch, ENABLE);
    TMR0_HWClearCondCmd(TMR0x, u32Ch, ENABLE);
}


void bsp_uart4_init(uint32_t baud)
{
	int32_t i32Ret;
	stc_dma_init_t stcDmaInit;
	stc_dma_llp_init_t stcDmaLlpInit;
	static stc_dma_llp_descriptor_t stcLlpDesc;
	stc_usart_uart_init_t stcUartInit;
	stc_irq_signin_config_t stcIrqSigninConfig;
	stc_gpio_init_t stcGpioInit;

	/* MCU Peripheral registers write unprotected */
	LL_PERIPH_WE(LL_PERIPH_SEL);
	
	// GPIO_SetDebugPort(GPIO_PIN_TRST|GPIO_PIN_TDO,DISABLE);//PB3/PB4 默认是TDO,NTRST功能，需要关闭特殊功能

	/* DMA&AOS FCG enable */
	RX4_DMA_FCG_ENABLE();
	TX4_DMA_FCG_ENABLE();
	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);

	/* USART_RX_DMA */
	uint32_t rxd_addr = &(UART4_UNIT->DR);
	(void)DMA_StructInit(&stcDmaInit);
	stcDmaInit.u32IntEn = DMA_INT_ENABLE;
	stcDmaInit.u32BlockSize = 1UL;
	stcDmaInit.u32TransCount = 0;//ARRAY_SZ(u8Rx4DmaBuf);
	stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
	stcDmaInit.u32DestAddr = (uint32_t)u8Rx4DmaBuf;
	stcDmaInit.u32SrcAddr = ((uint32_t)(&(UART4_UNIT->DR)) + 2UL);
	stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_FIX;
	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;
	i32Ret = DMA_Init(RX4_DMA_UNIT, RX4_DMA_CH, &stcDmaInit);
	if (LL_OK == i32Ret)
	{	
		/* 设置DMA为循环接收模式（无传输完成中断） ---------*/
		stc_dma_repeat_init_t stcDmaRepeatInit;
		(void)DMA_RepeatStructInit(&stcDmaRepeatInit);		
		
		stcDmaRepeatInit.u32Mode= DMA_RPT_DEST;
		stcDmaRepeatInit.u32SrcCount= 0;
		stcDmaRepeatInit.u32DestCount= ARRAY_SZ(u8Rx4DmaBuf);
		DMA_RepeatInit(RX4_DMA_UNIT, RX4_DMA_CH, &stcDmaRepeatInit);
		
		stc_dma_reconfig_init_t stcDmaReconfigInit;
		(void)DMA_ReconfigStructInit(&stcDmaReconfigInit);
		
		stcDmaReconfigInit.u32CountMode      = DMA_RC_CNT_DEST;
		stcDmaReconfigInit.u32SrcAddrMode    = DMA_RC_SRC_ADDR_KEEP;
		stcDmaReconfigInit.u32DestAddrMode   = DMA_RC_DEST_ADDR_RPT;
		(void)DMA_ReconfigInit(RX4_DMA_UNIT, RX4_DMA_CH, &stcDmaReconfigInit);

		AOS_SetTriggerEventSrc(AOS_DMA_RC, RX4_DMA_RC_EVT_SRC);
		DMA_ReconfigCmd(RX4_DMA_UNIT, ENABLE);
		
		AOS_SetTriggerEventSrc(RX4_DMA_TRIG_SEL, RX4_DMA_TRIG_EVT_SRC);
		
		DMA_Cmd(RX4_DMA_UNIT, ENABLE);
		(void)DMA_ChCmd(RX4_DMA_UNIT, RX4_DMA_CH, ENABLE);		
		
		Rx4DmaFront= 0;
		Rx4DmaRear= 0;
	}

	/* USART_TX_DMA */
	(void)DMA_StructInit(&stcDmaInit);
	stcDmaInit.u32IntEn = DMA_INT_ENABLE;
	stcDmaInit.u32BlockSize = 1UL;
	stcDmaInit.u32TransCount = ARRAY_SZ(u8Tx4DmaBuf);
	stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
	stcDmaInit.u32DestAddr = (uint32_t)(&(UART4_UNIT->DR));
	stcDmaInit.u32SrcAddr = (uint32_t)u8Tx4DmaBuf;
	stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;
	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;
	i32Ret = DMA_Init(TX4_DMA_UNIT, TX4_DMA_CH, &stcDmaInit);
	if (LL_OK == i32Ret)
	{
		/* DMA_TX传输完成中断使能 ---------------------*/
		stcIrqSigninConfig.enIntSrc = TX4_DMA_TC_INT_SRC;
		stcIrqSigninConfig.enIRQn  = TX4_DMA_TC_IRQn;
		stcIrqSigninConfig.pfnCallback = &TX4_DMA_TC_IrqCallback;
		INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

		AOS_SetTriggerEventSrc(TX4_DMA_TRIG_SEL, TX4_DMA_TRIG_EVT_SRC);

		DMA_Cmd(TX4_DMA_UNIT, ENABLE);
		DMA_TransCompleteIntCmd(TX4_DMA_UNIT, TX4_DMA_TC_INT, ENABLE);
	}

	uart_rx_timout_config(UART4_UNIT, UART4_TIMEOUT_BITS);

	(void)GPIO_StructInit(&stcGpioInit);
	stcGpioInit.u16PullUp= PIN_PU_ON;
	GPIO_Init(UART4_RX_PORT, UART4_RX_PIN, &stcGpioInit);
	
	GPIO_SetFunc(UART4_RX_PORT, UART4_RX_PIN, UART4_RX_GPIO_FUNC);
	GPIO_SetFunc(UART4_TX_PORT, UART4_TX_PIN, UART4_TX_GPIO_FUNC);

	UART4_FCG_ENABLE();
	uart4_ringbuff_init();

	/* Initialize UART. */
	(void)USART_UART_StructInit(&stcUartInit);
	stcUartInit.u32ClockDiv = USART_CLK_DIV1;
	stcUartInit.u32CKOutput = USART_CK_OUTPUT_ENABLE;
	stcUartInit.u32Baudrate = baud;
	stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_8BIT;
	if (LL_OK != USART_UART_Init(UART4_UNIT, &stcUartInit, NULL))
	{
//        for (;;) {
//        }
	}

	/* UART_TX完成中断使能 -----------------------*/
	stcIrqSigninConfig.enIRQn = UART4_TX_CPLT_IRQn;
	stcIrqSigninConfig.enIntSrc = UART4_TX_CPLT_INT_SRC;
	stcIrqSigninConfig.pfnCallback = &UART4_TxComplete_IrqCallback;
	INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

	/* UART_RX错误中断使能 -----------------------*/
	stcIrqSigninConfig.enIRQn = UART4_RX_ERR_IRQn;
	stcIrqSigninConfig.enIntSrc = UART4_RX_ERR_INT_SRC;
	stcIrqSigninConfig.pfnCallback = &UART4_RxError_IrqCallback;
	INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

	/* UART_RX超时（空闲）中断使能 ---------------*/
	stcIrqSigninConfig.enIRQn = UART4_RX_TIMEOUT_IRQn;
	stcIrqSigninConfig.enIntSrc = UART4_RX_TIMEOUT_INT_SRC;
	stcIrqSigninConfig.pfnCallback = &UART4_RxTimeout_IrqCallback;
	INTC_IrqInstalHandler(&stcIrqSigninConfig, UART4_RX_IRQ_PRIO);

	/* MCU Peripheral registers write protected */
	LL_PERIPH_WP(LL_PERIPH_SEL);

	/* Enable TX && RX && RX interrupt function */
	USART_FuncCmd(UART4_UNIT, (USART_RX | USART_INT_RX | USART_RX_TIMEOUT | \
	                           USART_INT_RX_TIMEOUT ), ENABLE);

	enTx4FrameEnd= SET;
}


void bsp_uart4_rx_chain_init(uint32_t baud)
{
	int32_t i32Ret;
	stc_dma_init_t stcDmaInit;
	stc_dma_llp_init_t stcDmaLlpInit;
	static stc_dma_llp_descriptor_t stcLlpDesc;
	stc_usart_uart_init_t stcUartInit;
	stc_irq_signin_config_t stcIrqSigninConfig;
	stc_gpio_init_t stcGpioInit;

	/* MCU Peripheral registers write unprotected */
	LL_PERIPH_WE(LL_PERIPH_SEL);
	
	// GPIO_SetDebugPort(GPIO_PIN_TRST|GPIO_PIN_TDO,DISABLE);//PB3/PB4 默认是TDO,NTRST功能，需要关闭特殊功能

	/* DMA&AOS FCG enable */
	RX4_DMA_FCG_ENABLE();
	TX4_DMA_FCG_ENABLE();
	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);

    /* USART_RX_DMA , 使用DMA连锁传输 */
    uint32_t rxd_addr = &(UART4_UNIT->DR);
    (void)DMA_StructInit(&stcDmaInit);
    stcDmaInit.u32IntEn = DMA_INT_ENABLE;
    stcDmaInit.u32BlockSize = 1UL;
    stcDmaInit.u32TransCount = UART4_RX_BUF_LEN;  // 修改为缓冲区大小
    stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32DestAddr = (uint32_t)u8Rx4DmaBuf;
    stcDmaInit.u32SrcAddr = ((uint32_t)(&(UART4_UNIT->DR)) + 2UL);
    stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;
    i32Ret = DMA_Init(RX4_DMA_UNIT, RX4_DMA_CH, &stcDmaInit);
    if (LL_OK == i32Ret)
    {   
        /* 配置DMA连锁传输 */
        (void)DMA_LlpStructInit(&stcDmaLlpInit);
        stcDmaLlpInit.u32State = DMA_LLP_ENABLE;
        stcDmaLlpInit.u32Mode = DMA_LLP_WAIT;
        stcDmaLlpInit.u32Addr = (uint32_t)&stcLlpDesc;
        DMA_LlpInit(RX4_DMA_UNIT, RX4_DMA_CH, &stcDmaLlpInit);
        
        /* 配置连锁描述符 */
        stcLlpDesc.SARx = stcDmaInit.u32SrcAddr;
        stcLlpDesc.DARx = stcDmaInit.u32DestAddr;
        stcLlpDesc.DTCTLx = (stcDmaInit.u32BlockSize << DMA_DTCTL_BLKSIZE_POS) |
                            (stcDmaInit.u32TransCount << DMA_DTCTL_CNT_POS);
        stcLlpDesc.CHCTLx = stcDmaInit.u32SrcAddrInc | stcDmaInit.u32DestAddrInc | 
                            stcDmaInit.u32DataWidth | stcDmaInit.u32IntEn |
                            stcDmaLlpInit.u32State | stcDmaLlpInit.u32Mode;
        stcLlpDesc.LLPx = (uint32_t)&stcLlpDesc;  // 指向自身，形成循环
        DMA_ReconfigLlpCmd(RX4_DMA_UNIT, RX4_DMA_CH, ENABLE);
        DMA_ReconfigCmd(RX4_DMA_UNIT, ENABLE);
        AOS_SetTriggerEventSrc(RX4_DMA_RECONF_TRIG_SEL, RX4_DMA_RECONF_TRIG_EVT_SRC);

        stcIrqSigninConfig.enIntSrc = UART4_RX_DMA_TC_INT_SRC;
        stcIrqSigninConfig.enIRQn = UART4_RX_DMA_TC_IRQn;
        stcIrqSigninConfig.pfnCallback = &UART4_RxDMA_TC_IrqCallback; 
        INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);
        NVIC_ClearPendingIRQ(stcIrqSigninConfig.enIRQn);
        NVIC_SetPriority(stcIrqSigninConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
        NVIC_EnableIRQ(stcIrqSigninConfig.enIRQn);

        AOS_SetTriggerEventSrc(RX4_DMA_TRIG_SEL, RX4_DMA_TRIG_EVT_SRC);

        DMA_Cmd(RX4_DMA_UNIT, ENABLE);
        DMA_TransCompleteIntCmd(RX4_DMA_UNIT, RX4_DMA_TC_INT, ENABLE);
        (void)DMA_ChCmd(RX4_DMA_UNIT, RX4_DMA_CH, ENABLE);        
        
        Rx4DmaFront = 0;
        Rx4DmaRear = 0;
    }

	/* USART_TX_DMA */
	(void)DMA_StructInit(&stcDmaInit);
	stcDmaInit.u32IntEn = DMA_INT_ENABLE;
	stcDmaInit.u32BlockSize = 1UL;
	stcDmaInit.u32TransCount = ARRAY_SZ(u8Tx4DmaBuf);
	stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
	stcDmaInit.u32DestAddr = (uint32_t)(&(UART4_UNIT->DR));
	stcDmaInit.u32SrcAddr = (uint32_t)u8Tx4DmaBuf;
	stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;
	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;
	i32Ret = DMA_Init(TX4_DMA_UNIT, TX4_DMA_CH, &stcDmaInit);
	if (LL_OK == i32Ret)
	{
		/* DMA_TX传输完成中断使能 ---------------------*/
		stcIrqSigninConfig.enIntSrc = TX4_DMA_TC_INT_SRC;
		stcIrqSigninConfig.enIRQn  = TX4_DMA_TC_IRQn;
		stcIrqSigninConfig.pfnCallback = &TX4_DMA_TC_IrqCallback;
		INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

		AOS_SetTriggerEventSrc(TX4_DMA_TRIG_SEL, TX4_DMA_TRIG_EVT_SRC);

		DMA_Cmd(TX4_DMA_UNIT, ENABLE);
		DMA_TransCompleteIntCmd(TX4_DMA_UNIT, TX4_DMA_TC_INT, ENABLE);
	}

	uart_rx_timout_config(UART4_UNIT, UART4_TIMEOUT_BITS);

	(void)GPIO_StructInit(&stcGpioInit);
	stcGpioInit.u16PullUp= PIN_PU_ON;
	GPIO_Init(UART4_RX_PORT, UART4_RX_PIN, &stcGpioInit);
	
	GPIO_SetFunc(UART4_RX_PORT, UART4_RX_PIN, UART4_RX_GPIO_FUNC);
	GPIO_SetFunc(UART4_TX_PORT, UART4_TX_PIN, UART4_TX_GPIO_FUNC);

	UART4_FCG_ENABLE();
	uart4_ringbuff_init();

	/* Initialize UART. */
	(void)USART_UART_StructInit(&stcUartInit);
	stcUartInit.u32ClockDiv = USART_CLK_DIV1;
	stcUartInit.u32CKOutput = USART_CK_OUTPUT_ENABLE;
	stcUartInit.u32Baudrate = baud;
	stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_8BIT;
	if (LL_OK != USART_UART_Init(UART4_UNIT, &stcUartInit, NULL))
	{
//        for (;;) {
//        }
	}

	/* UART_TX完成中断使能 -----------------------*/
	stcIrqSigninConfig.enIRQn = UART4_TX_CPLT_IRQn;
	stcIrqSigninConfig.enIntSrc = UART4_TX_CPLT_INT_SRC;
	stcIrqSigninConfig.pfnCallback = &UART4_TxComplete_IrqCallback;
	INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

	/* UART_RX错误中断使能 -----------------------*/
	stcIrqSigninConfig.enIRQn = UART4_RX_ERR_IRQn;
	stcIrqSigninConfig.enIntSrc = UART4_RX_ERR_INT_SRC;
	stcIrqSigninConfig.pfnCallback = &UART4_RxError_IrqCallback;
	INTC_IrqInstalHandler(&stcIrqSigninConfig, DDL_IRQ_PRIO_DEFAULT);

	/* UART_RX超时（空闲）中断使能 ---------------*/
	stcIrqSigninConfig.enIRQn = UART4_RX_TIMEOUT_IRQn;
	stcIrqSigninConfig.enIntSrc = UART4_RX_TIMEOUT_INT_SRC;
	stcIrqSigninConfig.pfnCallback = &UART4_RxTimeout_IrqCallback;
	INTC_IrqInstalHandler(&stcIrqSigninConfig, UART4_RX_IRQ_PRIO);

	/* MCU Peripheral registers write protected */
	LL_PERIPH_WP(LL_PERIPH_SEL);

	/* Enable TX && RX && RX interrupt function */
	USART_FuncCmd(UART4_UNIT, (USART_RX | USART_INT_RX | USART_RX_TIMEOUT | \
	                           USART_INT_RX_TIMEOUT ), ENABLE);

	enTx4FrameEnd= SET;
}
#endif



#ifdef __cplusplus
}
#endif


