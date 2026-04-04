#include "RingBuffer.h"

/**
 * @details Initializes the Ring buffer.
 * @param Rb     The ring buffer to initialize
 * @param buffer The buffer allocated for the ring buffer
 * @param size   The size of the allocated ring buffer needs to be POWER OF TWO
 */
void RingBuffer_Init(RingBuffer_t *Rb, uint8_t *buffer, uint32_t size)
{

    Rb->buffer = buffer;
    Rb->read_index = 0;
    Rb->write_index = 0;
    Rb->mask = size - 1;
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
 * @returns BUFFER_FULL if full; BUFFER_NOT_FULL otherwise
 */
bool RingBuffer_Full(RingBuffer_t *Rb)
{
    uint32_t next_write_index = ((Rb->write_index + 1) & Rb->mask );

    return (next_write_index == Rb->read_index);
}

/**
 * @details Get a byte from the Queue, (TAIL)
 */
RingBuffer_Status RingBuffer_Read(RingBuffer_t *Rb, uint8_t *byte)
{
    uint32_t local_read_index = Rb->read_index;
    uint32_t local_write_index = Rb->write_index;

    if (local_write_index == local_read_index)               // We don't have any data to read -> Ring buffer empty
    {
        return BUFFER_EMPTY;
    }

    *byte = Rb->buffer[local_read_index];
    local_read_index = (local_read_index + 1) & Rb->mask;
    Rb->read_index = local_read_index;

    return BUFFER_OK;
}



/**
 * @details Put a byte to a ring buffer, (HEAD)
 * @param Rb The buffer in which the data shjould be put
 * @param byte The byte to put in the ring buffer
 */
RingBuffer_Status RingBuffer_Write(RingBuffer_t *Rb, uint8_t byte)
{
    
    uint32_t local_read_index = Rb->read_index;
    uint32_t local_write_index = Rb->write_index;
    uint32_t next_write_index = (Rb->write_index + 1) & Rb->mask;
    
    if(next_write_index == local_read_index)
    {
        return BUFFER_FULL;
    }

    Rb->buffer[local_write_index] = byte;                           
    Rb->write_index = next_write_index;
    return BUFFER_OK;
}


/**
 * @details Number of bytes available for reading
 */
uint32_t RingBuffer_Available(RingBuffer_t *Rb)
{
    return (Rb->write_index - Rb->read_index) & Rb->mask;
}

/**
 * @details Free space available for writing
 */
uint32_t RingBuffer_FreeSpace(RingBuffer_t *Rb)
{
    return ((Rb->write_index - Rb->read_index) - 1) & Rb->mask;
}