#ifndef CRC_H
#define CRC_H

#include "Common_Defines.h"

uint8_t Crc8_Compute(uint8_t *data, uint8_t length);
uint32_t Crc32_Compute(uint8_t *data, uint32_t length);

#endif // CRC_H