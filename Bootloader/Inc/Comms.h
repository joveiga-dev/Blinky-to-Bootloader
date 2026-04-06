#ifndef COMMS_H
#define COMMS_H

#include "Common_Defines.h"

#define PACKET_DATA_LENGTH (16)
#define PACKET_LENGTH_BYTES (1)
#define PACKET_CRC_BYTES (1)
#define PACKET_LENGTH_MAX (PACKET_LENGTH_BYTES + PACKET_DATA_LENGTH + PACKET_CRC_BYTES)
#define PACKET_ACK_DATA_0 (0X15)
#define PACKET_RETX_DATA_0 (0X19)

typedef struct
{
    uint8_t length;
    uint8_t data[PACKET_DATA_LENGTH];
    uint8_t crc;
} Comms_Packet_t;

// TODO: Provide an interface for the transport handle instead of directly using USART_Handle
void Comms_Setup(void);
void Comms_Update(USART_Handle *USARTx);
bool Comms_Packets_Available(void);
void Comms_Send(USART_Handle *USARTx, const Comms_Packet_t *Packet);    // Write
void Comms_Receive(USART_Handle *USARTx, const Comms_Packet_t *Packet); // Read
uint8_t Comms_Compute_CRC(Comms_Packet_t *Packet);

#endif // COMMS_H