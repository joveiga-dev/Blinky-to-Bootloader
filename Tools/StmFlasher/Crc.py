
# CRC-8 Computation
def Crc8_Compute(data) ->int :
    crc = 0
    poly = 0x07
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if (crc & 0x80):
                crc = (crc << 1) ^ poly
            else:
                crc <<= 1
            crc &= 0xFF
    return crc

# CRC-32 Computation

def Crc32_Compute(data):
    crc = 0xFFFFFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if (crc & 1):
                crc = (crc >> 1) ^ 0xEDB88320
            else:
                crc >>= 1
            crc &= 0xFFFFFFFF
    return (~crc)
    
    
    