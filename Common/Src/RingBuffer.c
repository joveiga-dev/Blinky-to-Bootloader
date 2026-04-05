#include "RingBuffer.h"

/**
 * @brief Initializes the Ring buffer.
 * @param Rb     The Pointer ring buffer to initialize
 * @param buffer The buffer pre-allocated memory block
 * @param size   The size of the allocated ring buffer needs to be POWER OF TWO
 */
void RingBuffer_Init(RingBuffer_t *Rb, uint8_t *buffer, uint32_t size)
{
    if (Rb == NULL) return;
    Rb->buffer = buffer;
    Rb->read_index = 0;
    Rb->write_index = 0;
    Rb->mask = size - 1;
}

/**
 * @brief Determines whether or not the Ring buffer is empty
 * @returns true if empty; false otherwise
 */
bool RingBuffer_Empty(RingBuffer_t *Rb)
{
    return (Rb->write_index == Rb->read_index);
}


/**
 * @brief Determines whether or not the RB is Full
 * @returns true if full; false otherwise
 */
bool RingBuffer_Full(RingBuffer_t *Rb)
{
    uint32_t next_write_index = ((Rb->write_index + 1) & Rb->mask );

    return (next_write_index == Rb->read_index);
}

/**
 * @brief Read a byte from the buffer, (TAIL)
 * @param byte  Pointer to store the retrived byte
 * @return RINGBUFFER_OK on success, RINGBUFFER_EMPTY if no data
 */
RingBuffer_Status_t RingBuffer_Read(RingBuffer_t *Rb, uint8_t *byte)
{
    uint32_t local_read_index = Rb->read_index;
    uint32_t local_write_index = Rb->write_index;
    // Check if empty
    if (local_write_index == local_read_index)               
    {
        return RINGBUFFER_EMPTY;
    }

    // Retrieve data from tail and wrap read index
    if (byte != NULL)
    {
        *byte = Rb->buffer[local_read_index];
    }
    
    local_read_index = (local_read_index + 1) & Rb->mask;
    Rb->read_index = local_read_index;
    return RINGBUFFER_OK;
}



/**
 * @details Put a byte to a ring buffer, (HEAD)
 * @param Rb The buffer in which the data shjould be put
 * @param byte The byte to put in the ring buffer
 */
/**
 * @brief Writes a byte to the buffer, (HEAD)
 * @param byte  The byte to place in the buffer
 * @return RINGBUFFER_OK on success, RINGBUFFER_FULL if no space
 */
RingBuffer_Status_t RingBuffer_Write(RingBuffer_t *Rb, uint8_t byte)
{
    
    uint32_t local_read_index = Rb->read_index;
    uint32_t local_write_index = Rb->write_index;
    uint32_t next_write_index = (Rb->write_index + 1) & Rb->mask;
    
    if(next_write_index == local_read_index)
    {
        return RINGBUFFER_FULL;
    }

    // Add data at the head and wrap write index
    Rb->buffer[local_write_index] = byte;                           
    Rb->write_index = next_write_index;
    return RINGBUFFER_OK;
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