

# CRC-8 Computation
def Crc8_Compute(data):
    crc = 0x00
    poly = 0x07

    for byte in data:
        crc = (crc ^ byte)
        for _ in range(8):
            if crc & 0x80:
                crc = (crc << 1) ^ poly
            else:
                crc = (crc << 1)
            crc &=0xff
            
    #print(f"Client CRC Input: {data.hex(' ')} : Client Computed CRC: {hex(crc)}")

    return crc

def crc8(data):
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x80:
                crc = (crc << 1) ^ 0x07
            else:
                crc <<= 1
    return (crc & 0xFF)



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
    
    
    