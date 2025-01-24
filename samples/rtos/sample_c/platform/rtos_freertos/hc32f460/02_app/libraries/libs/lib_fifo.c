/*
 * lib_fifo.c
 *
 * Created: 2016/4/19 16:46:25
 *  Author: Bamboo
 */
//#include <stm32f4xx.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <lib_fifo.h>


#define RINGBUFFER_LOCK             __disable_irq
#define RINGBUFFER_UNLOCK           __enable_irq

#define CRITICAL_ENTER              __disable_irq
#define CRITICAL_EXIT               __enable_irq

int RingBuffer_available_data(RingBuffer *buffer)
{
    return buffer->len;

    RINGBUFFER_LOCK();

    int datalen = (buffer->end >= buffer->start) ? (buffer->end - buffer->start) : ((buffer->length - buffer->start) + buffer->end);

    RINGBUFFER_UNLOCK();

    return datalen;
}

int RingBuffer_available_space(RingBuffer *buffer)
{
    return buffer->length - buffer->len;

    RINGBUFFER_LOCK();

    int free_len = (buffer->end >= buffer->start) ? ((buffer->length - buffer->end) + buffer->start) : (buffer->start - buffer->end);

    RINGBUFFER_UNLOCK();

    return free_len;
}


int RingBuffer_create(RingBuffer *buffer, uint32_t fifosize)
{
    buffer->length  = fifosize;
    buffer->start = 0;
    buffer->end = 0;
    buffer->len = 0;

    buffer->intNum = (uint32_t)-1;

    CRITICAL_ENTER();
    if(NULL == buffer->buffer) {
        buffer->buffer = malloc(fifosize);
    }
    RINGBUFFER_UNLOCK();

    if(buffer->buffer == NULL)
        return -1;

    return 0;
}

void RingBuffer_destroy(RingBuffer *buffer)
{
    if(buffer) {
        free(buffer->buffer);
    }
}

int RingBuffer_write(RingBuffer *buffer, const uint8_t *data, int len)
{
    unsigned int available_space = RingBuffer_available_space(buffer);
    if(len > available_space)
    {
//        printf("%s %d:write buffer error len=%d,space=%d\r\n", __FUNCTION__, __LINE__, len, available_space);
        return -1;
    }

    int write_len = (buffer->length - buffer->end) >= len ? len : (buffer->length - buffer->end);
    memcpy(&buffer->buffer[buffer->end], data, write_len);

    if((len - write_len) > 0){
        memcpy(buffer->buffer, data + write_len, (len - write_len));
    }

    RINGBUFFER_LOCK();
    RingBuffer_commit_write(buffer, len);
    buffer->len += len;
    RINGBUFFER_UNLOCK();

    return len;
}

int RingBuffer_read(RingBuffer *buffer, uint8_t *target, int len)
{
    int available_data = RingBuffer_available_data(buffer);
    if(available_data <= 0)
    {
        return 0;
    }

    if(len < 0 || len > available_data){
        len = available_data;
    }

    int read_len = ((buffer->length - buffer->start) >= len) ? len : (buffer->length - buffer->start);

    memcpy(target, &buffer->buffer[buffer->start], read_len);
    if(len > read_len){
        memcpy(target + read_len, buffer->buffer, (len - read_len));
    }

    RINGBUFFER_LOCK();
    RingBuffer_commit_read(buffer, len);
    buffer->len -= len;
    RINGBUFFER_UNLOCK();

    return len;
}

void RingBuffer_reset(RingBuffer *buffer)
{
    RINGBUFFER_LOCK();
    buffer->start = 0;
    buffer->end = 0;
    buffer->len = 0;
    if(buffer->end == buffer->start) {
        buffer->start = buffer->end = 0;
    }
    RINGBUFFER_UNLOCK();
}

