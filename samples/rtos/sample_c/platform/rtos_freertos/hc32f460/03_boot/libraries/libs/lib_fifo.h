/*
 * lib_fifo.h
 *
 * Created: 2016/4/19 16:35:54
 *  Author: Bamboo
 */


#ifndef LIBS_FIFO_H_
#define LIBS_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ring buffer
typedef struct {
            uint8_t *buffer;
            int length;
volatile    int start;
volatile    int end;
uint32_t	intNum;
volatile unsigned int len;//增加一个成员表示buffer有效数据长度，避免buffer满和空时判断错误
} RingBuffer;

int RingBuffer_create(RingBuffer *buffer, uint32_t fifosize);

void RingBuffer_destroy(RingBuffer *buffer);

int RingBuffer_read(RingBuffer *buffer, uint8_t *target, int amount);

int RingBuffer_write(RingBuffer *buffer, const uint8_t *data, int length);

void RingBuffer_reset(RingBuffer *buffer);

int RingBuffer_empty(RingBuffer *buffer);

int RingBuffer_full(RingBuffer *buffer);

int RingBuffer_available_data(RingBuffer *buffer);

int RingBuffer_available_space(RingBuffer *buffer);

//#define RingBuffer_available_data(B) (((B)->end + 1) % (B)->length - (B)->start - 1)
//#define RingBuffer_available_data(B) (((B)->end + 1) - (B)->start - 1)
//
//#define RingBuffer_available_space(B) ((B)->length - (B)->end - 1)

#define RingBuffer_full(B) (RingBuffer_available_data((B)) - (B)->length == 0)

#define RingBuffer_empty(B) (RingBuffer_available_data((B)) == 0)

#define RingBuffer_puts(B, D) RingBuffer_write((B), bdata((D)), blength((D)))

#define RingBuffer_get_all(B) RingBuffer_gets((B), RingBuffer_available_data((B)))

#define RingBuffer_starts_at(B) ((B)->buffer + (B)->start)

#define RingBuffer_ends_at(B) ((B)->buffer + (B)->end)

#define RingBuffer_commit_read(B, A) ((B)->start = ((B)->start + (A)) % (B)->length)

#define RingBuffer_commit_write(B, A) ((B)->end = ((B)->end + (A)) % (B)->length)

#ifdef __cplusplus
}
#endif


#endif /* LIB_FIFO_H_ */


