#include "Ring_Buffer.h"

/**
 * @details Initializes the Ring buffer.
 * @param Rb     The ring buffer to initialize
 * @param buffer The buffer allocated for the ring buffer
 * @param size   The size of the allocated ring buffer needs to be POWER OF TWO
 */
void RingBuffer_Init(RingBuffer_t *Rb, uint8_t *buffer, uint32_t size)
{
    if (RINGBUFFER_POWER_OF_TWO(size) != 0)
    {
        return;
    }

    Rb->buffer = buffer;
    Rb->read_index = 0;
    Rb->write_index = 0;
    Rb->mask = size - 1;
    Rb->status = BUFFER_EMPTY;
}

/**
 * @details Determines whether or not the Ring buffer is empty
 * @param Rb The buffer
 * @returns true if empty; false otherwise
 */
bool RingBuffer_Empty(RingBuffer_t *Rb)
{
    return (Rb->write_index == Rb->read_index);
}


/**
 * @details Determines whether or not the Queue is Full
 * @param Rb The ring buffer
 * @returns true if full; false otherwise
 */
bool RingBuffer_Full(RingBuffer_t *Rb)
{
    return (((Rb->write_index +1) & Rb->mask) == (Rb->read_index)) ;
}

/**
 * @details Get a byte from the Queue, (TAIL)
 */
bool RingBuffer_Read(RingBuffer_t *Rb, uint8_t *byte)
{
    uint32_t next_read_index;

    if (Rb->write_index == Rb->read_index)               // We don't have any data to read -> Ring buffer empty
    {
        return false;
    }

    next_read_index = ((Rb->read_index + 1) & Rb->mask);

    *byte = Rb->buffer[Rb->read_index];
    Rb->read_index = next_read_index;
    return true;
}

bool RingBuffer_ReadArr(RingBuffer_t *Rb, const uint8_t *byte, uint32_t size)
{
    if(Rb->write_index == Rb->read_index)
    {
        return false;
    }

    uint32_t count = 0;


    for (count = 0; count < size; count++)
    {
        RingBuffer_Read(Rb, &byte[count]);
    }
    
    return true;
    
}


/**
 * @details Put a byte to a ring buffer, (HEAD)
 * @param Rb The buffer in which the data shjould be put
 * @param byte The byte to put in the ring buffer
 */
bool RingBuffer_Write(RingBuffer_t *Rb, uint8_t byte)
{
    uint32_t local_write_index = Rb->write_index;
    uint32_t local_read_index = Rb->read_index;

    uint32_t next_write_index = (Rb->write_index + 1) & Rb->mask;   // next where head will point to after this write

    if (next_write_index == local_read_index)                         // If head +1 == tail, buffer is full
    {
        return false;
        // Raise an exception or
        // Overwrite the oldest byte
    }

    Rb->buffer[Rb->write_index] = byte;                              // Load data
    Rb->write_index = next_write_index;
    return true;
}

/**
 * @details Put na array of bytes into buffer
 * @param Rb The buffer
 * @param byte A pointer to the array 
 * @param size The size of the array
 */
bool RingBuffer_WriteArr(RingBuffer_t *Rb, const uint8_t *byte, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        RingBuffer_Write(Rb, byte[i]);
    }
    
}
