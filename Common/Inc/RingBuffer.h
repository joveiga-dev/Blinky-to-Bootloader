#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "Common_Defines.h"

#define RINGBUFFER_POWER_OF_TWO(x) ((x) && !(((x) & ((x) - 1)) == 0))

typedef enum RingBuffer_Status
{
    BUFFER_OK = 0,
    BUFFER_EMPTY,
    BUFFER_FULL,
} RingBuffer_Status;

/**
 * Structure which holds a ring buffer
 */
typedef struct RingBuffer_t
{
    uint8_t * buffer;                   // Buffer memory
    uint32_t mask;                      // Buffer mask
    uint32_t write_index;               // Write to buffer index (Head)
    uint32_t read_index;                // Read to buffer index (Tail)
} RingBuffer_t;

// APIs
void RingBuffer_Init(RingBuffer_t *Rb, uint8_t *buffer, uint32_t size);
bool RingBuffer_Empty(RingBuffer_t *Rb);
bool RingBuffer_Full(RingBuffer_t *Rb);

RingBuffer_Status RingBuffer_Write(RingBuffer_t *Rb, uint8_t byte);
RingBuffer_Status RingBuffer_Read(RingBuffer_t *Rb, uint8_t *byte);

uint32_t RingBuffer_Available(RingBuffer_t *Rb);
uint32_t RingBuffer_FreeSpace(RingBuffer_t *Rb);


#endif