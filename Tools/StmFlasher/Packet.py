from Crc import Crc8_Compute, Crc32_Compute
from Uart import UartComms
from Const import (
    PACKET_CRC_BYTES, PACKET_DATA_BYTES, PACKET_LENGTH_BYTES, PACKET_CRC_INDEX, PACKET_ACK_DATA0, 
    PACKET_RETX_DATA0, PACKET_TOTAL_BYTES
)

class Packet:
    
    def __init__(self, length:int, data:bytes, crc=None)->None:
        self.length = length
        pad_len = PACKET_DATA_BYTES - len(data)
        self.data = data + bytes([0xFF for _ in range(pad_len)])
        self.crc = crc if crc is not None else self.compute_crc()

    def compute_crc(self, model: str = "Crc8"):
        if model == "Crc8":
            return Crc8_Compute(bytes([self.length]) + self.data)
        elif model == "Crc32":
            return Crc32_Compute(bytes([self.length]) + self.data)
        else:
            raise ValueError(f"Crc Algorithm not allowed {model}")
        
    def to_bytes(self):
        return (bytes([self.length]) + self.data + bytes([self.crc]))
    
    def is_packet_single_byte(self, byte):
        if(self.length != 1):
            return False
        if(self.data[0] != byte):
            return False
        for b in self.data[1:]:
            if(b != 0xFF):
                return False
        return True
    
    def is_packet_ReTx(self):
        return self.is_packet_single_byte(PACKET_RETX_DATA0)
    def is_packet_Ack(self):
        return self.is_packet_single_byte(PACKET_ACK_DATA0)
    
    @classmethod
    def single_byte_packet(cls, byte):
        return cls(1, bytes([byte]))
    @classmethod
    def Ack_packet(cls):
        return cls.single_byte_packet(PACKET_ACK_DATA0)
    @classmethod
    def Retx_packet(cls):
        return cls.single_byte_packet(PACKET_RETX_DATA0)
    
    
        
        