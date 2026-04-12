#include "Comms.h"
#include "RingBuffer.h"
#include "USerial.h"
#include "Crc.h"

#define PACKET_BUFFER_SIZE (8)

typedef enum Comms_State_t
{
    COMMS_STATE_LENGTH,
    COMMS_STATE_DATA,
    COMMS_STATE_CRC
} Comms_State_t;

static Comms_State_t State = COMMS_STATE_LENGTH;
static uint8_t data_byte_count = 0;

static Comms_Packet_t Current_Packet = {.length = 0, .data = {0}, .crc = 0};
static Comms_Packet_t ReTx_Packet = {.length = 0, .data = {0}, .crc = 0};
static Comms_Packet_t Ack_Packet = {.length = 0, .data = {0}, .crc = 0};
static Comms_Packet_t ReTx_last_Packet = {.length = 0, .data = {0}, .crc = 0};

// Ring buffer
static Comms_Packet_t Packet_Buffer[PACKET_BUFFER_SIZE];
static uint32_t Packet_Buffer_Write_Index = 0;
static uint32_t Packet_Buffer_Read_Index = 0;
static uint32_t Packet_Buffer_Mask = PACKET_BUFFER_SIZE - 1;

/**
 * @brief Copies the contents of one packet to another
 * @param dest Pointer to the destination packet
 * @param src const Pointer to the source packet
 * @return void
 */
static void Comms_Packet_Copy(Comms_Packet_t *dest, const Comms_Packet_t *src)
{
    dest->length = src->length;
    for (uint8_t i = 0; i < PACKET_DATA_LENGTH; i++)
    {
        dest->data[i] = src->data[i];
    }
    dest->crc = src->crc;
}

/**
 * @brief Checks if the received packet is a retransmission or Ack packet request
 * @param Packet Pointer to the received packet
 * @param data The expected data byte
 * @return true if the packet is a single-byte packet with the expected data, false otherwise
 */
static bool Is_Single_Byte_Packet(const Comms_Packet_t *Packet, uint8_t data)
{
    if (Packet->length != 1)
    {
        return false;
    }

    if (Packet->data[0] != data)
    {
        return false;
    }

    for (uint8_t i = 1; i < PACKET_DATA_LENGTH; i++)
    {
        if (Packet->data[i] != 0xFF)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Initializes the communication module
 *  @return void
 */
void Comms_Setup(void)
{
    // Retransmission packet setup
    ReTx_Packet.length = 1;
    ReTx_Packet.data[0] = PACKET_RETX_DATA_0;
    for (uint8_t i = 1; i < PACKET_DATA_LENGTH; i++)
    {
        ReTx_Packet.data[i] = 0xFF;
    }
    ReTx_Packet.crc = Comms_Compute_CRC(&ReTx_Packet);

    // Ack packet setup
    Ack_Packet.length = 1;
    Ack_Packet.data[0] = PACKET_ACK_DATA_0;
    for (uint8_t i = 1; i < PACKET_DATA_LENGTH; i++)
    {
        Ack_Packet.data[i] = 0xFF;
    }
    Ack_Packet.crc = Comms_Compute_CRC(&Ack_Packet);
}

/**
 * @brief Updates the communication state machine, processes incoming data, and manages packet reception
 * @param USARTx Pointer to the USART handle for communication
 *  @return void
 */
void Comms_Update(USART_Handle *USARTx)
{
    while (Userial_Data_Available())
    {
        uint8_t rx = Userial_ReceiveByte();

        switch (State)
        {
        case COMMS_STATE_LENGTH:
            Current_Packet.length = rx;
            State = COMMS_STATE_DATA;
            break;

        case COMMS_STATE_DATA:
            Current_Packet.data[data_byte_count] = rx;
            data_byte_count++;
            if (data_byte_count >= PACKET_DATA_LENGTH)
            {
                data_byte_count = 0;
                break;
            }

            State = COMMS_STATE_CRC;
            data_byte_count = 0;
            break;

        case COMMS_STATE_CRC:
            Current_Packet.crc = rx;
            // Compute the expected crc and compare
            uint8_t expected_crc = Comms_Compute_CRC(&Current_Packet);
            if (expected_crc != Current_Packet.crc)
            {
                // Request a retransmission
                Comms_Send(USARTx, &ReTx_Packet);
                State = COMMS_STATE_LENGTH;
                data_byte_count = 0;
                break;
            }

            if (Is_Single_Byte_Packet(&Current_Packet, PACKET_RETX_DATA_0))
            {
                Comms_Send(USARTx, &ReTx_last_Packet);
            }
            else if (Is_Single_Byte_Packet(&Current_Packet, PACKET_ACK_DATA_0))
            {
                // ACK received
            }
            else
            {
                // Store the packet in a buffer
                uint32_t next_write_index = (Packet_Buffer_Write_Index + 1) & Packet_Buffer_Mask;
                if (next_write_index != Packet_Buffer_Read_Index)
                {
                    Comms_Packet_Copy(&Packet_Buffer[Packet_Buffer_Write_Index], &Current_Packet);
                    Packet_Buffer_Write_Index = next_write_index;
                }
                else
                {
                }
                // Send Ack
                Comms_Send(USARTx, &Ack_Packet);
            }
            State = COMMS_STATE_LENGTH;
            data_byte_count = 0;
            break;

        default:
            State = COMMS_STATE_LENGTH;
            data_byte_count = 0;
            break;
        }
    }
}

/**
 *@brief Checks if there are any packets available in the buffer
 * @return true if there are packets available, false otherwise
 */
bool Comms_Packets_Available(void)
{
    return Packet_Buffer_Read_Index != Packet_Buffer_Write_Index;
}

/**
 * @brief Sends a packet over the communication interface
 * @param USARTx Pointer to the USART handle for communication
 * @param Packet Pointer to the packet to be sent
 *  @return void
 */
void Comms_Send(USART_Handle *USARTx, Comms_Packet_t *Packet)
{
    if (USARTx == NULL || Packet == NULL)
    {
        return;
    }

    Userial_SendData(USARTx, (uint8_t *)Packet, PACKET_LENGTH_MAX);
    Comms_Packet_Copy(&ReTx_last_Packet, Packet);
}

/**
 * @brief Receives a packet from the communication interface
 * @param Packet Pointer to the packet structure where received data will be stored
 */
void Comms_Receive(USART_Handle *USARTx, Comms_Packet_t *Packet)
{
    if (USARTx == NULL || Packet == NULL)
    {
        return;
    }

    Comms_Packet_Copy(Packet, &Packet_Buffer[Packet_Buffer_Read_Index]);
    Packet_Buffer_Read_Index = (Packet_Buffer_Read_Index + 1) & Packet_Buffer_Mask;
}

/**
 * @brief Computes the CRC for a given packet
 * @param Packet Pointer to the packet for which to compute CRC
 * @return Computed CRC value
 */
uint8_t Comms_Compute_CRC(Comms_Packet_t *Packet)
{
    return Crc8_Compute((uint8_t *)Packet, PACKET_DATA_LENGTH + PACKET_LENGTH_BYTES);
}
