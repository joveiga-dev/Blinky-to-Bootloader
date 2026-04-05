#ifndef COMMS_H
#define COMMS_H

#include "Common_Defines.h"

#define PACKET_DATA_LENGTH    (16)
#define PACKET_LENGTH         (1)
#define PACKET_CRC_LENGTH         (1)

typedef struct
{
    uint8_t length;
    uint8_t data[PACKET_DATA_LENGTH];
    uint8_t crc;
} Comms_Packet_t;


void Comms_Setup(void);
void Comms_Update(void);
bool Comms_Packets_Available(void);
void Comms_Send(Comms_Packet_t *Packet); // Write
void Comms_Receive(Comms_Packet_t *Packet);  // Read
uint8_t Comms_Compute_CRC(Comms_Packet_t *Packet);

#endif // COMMS_H