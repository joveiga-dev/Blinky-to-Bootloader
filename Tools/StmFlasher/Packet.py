import threading
from queue import Queue, Empty
from enum import IntEnum
import struct
import time

from Crc import Crc8_Compute, Crc32_Compute
from Uart import UartComms

#Constants
PACKET_LENGTH_BYTES = 1
PACKET_DATA_BYTES   = 16
PACKET_CRC_BYTES    = 1
PACKET_TOTAL_BYTES  = PACKET_LENGTH_BYTES + PACKET_DATA_BYTES + PACKET_CRC_BYTES
PACKET_CRC_INDEX    = PACKET_LENGTH_BYTES + PACKET_DATA_BYTES

# Packet Type enum
class PacketType(IntEnum):
    Ack_Packet = 0x15
    ReTx_Packet = 0x19
    Nack_Packet = 0x59
    SOP = 0xA5

# Class for serializing and deserializing packets
class Packet:
    """
    Structure of the Packet:
        [length (1 byte) | data (16 bytes) | crc (1 byte)]
    """
    FORMAT = f'<BB{PACKET_DATA_BYTES}sB'

    def __init__(self, length: int, data:bytes, crc: int | None = None)->None:
        self.length = length
        self.data = data.ljust(PACKET_DATA_BYTES, b'\xff')[:PACKET_DATA_BYTES]
        self.crc = self.compute_crc() if crc is None else crc
        
    def compute_crc(self):
        payload = struct.pack(f'<BB16s', self.sop, self.length, self.data)
        crc = Crc8_Compute(payload)
        return crc
        
    def pack(self) ->bytes:
        return struct.pack(self.FORMAT, self.sop, self.length, self.data, self.crc)
    
    @classmethod
    def unpack(cls, buffer:bytes):
        sop,length, data_raw, crc_rec = struct.unpack(cls.FORMAT, buffer)
        obj = cls(length=length, data=data_raw, crc=None)
        if obj.compute_crc() != crc_rec:
            raise ValueError(f"Invalid CRC! Expected: {hex(obj.compute_crc())}, Received: {hex(crc_rec)}")
        obj.crc = crc_rec
        return obj
        
    def is_crc_valid(self)-> bool:
        return self.crc == self.compute_crc()
        
    def to_bytes(self):
        return self.pack()
    
    @staticmethod
    def from_bytes(raw: bytes)-> "Packet":
        return Packet.unpack(raw)
    
    def is_single_byte_packet(self, byte: int)->bool:
        return (
            self.length == 1 and 
            self.data[0] == byte and 
            all(b == 0xff for b in self.data[1:])
        )
    
    def is_ReTx(self):
        return self.is_single_byte_packet(PacketType.ReTx_Packet.value)
    def is_Ack(self):
        return self.is_single_byte_packet(PacketType.Ack_Packet.value)
    
    @staticmethod
    def single_byte_packet(byte: int) -> "Packet":
        return Packet(1, bytes([byte]))
    

#######################################################################
#######################################################################
class UartProtocol:
    """
    High-lvel protocol handler
    """
    
    def __init__(self, uart: UartComms): 
        self.uart = uart
        self.rx_buffer = bytearray()
        self.queue: Queue[Packet] = Queue()
        self.running = False
        self.thread = None
        self.last_packet = None
        
    def start(self):
        if not self.uart.ser or not self.uart.ser.is_open:
            raise ConnectionError("Serial not conencted.")
        self.running = True
        self.thread = threading.Thread(target=self._reader, daemon=True)
        self.thread.start()
    
    def stop(self):
        self.running = False
        if self.thread:
            self.thread.join(timeout=1)
    
    def send_packet(self, packet: Packet)->None:
        data = packet.to_bytes()
        self.uart.send_data(data)
        self.last_packet = packet
        
    def _reader(self):
        # Thread
        while self.running:
            try:
                data = self.uart.receive_data()
                #print("aqui", data)
                if data:
                    #print("[RX RAW]", data.hex(' '))  
                    self.rx_buffer.extend(data)
                    self._process()
                else:
                    time.sleep(0.001)
            except Exception as e:
                print("[ERROR]", e)
                self.running = False
        
    def _process(self):
        #print("here")
        while True:

            # 1. need at least header
            #print("here1")
            if len(self.rx_buffer) < 4:
                ##print("her2")
                return

            # 2. sync to SOP
            if self.rx_buffer[0] != PacketType.SOP.value:
                #print("sop")
                self.rx_buffer.pop(0)
                continue

            sop = self.rx_buffer[0]
            length = self.rx_buffer[1]

            total_size = 1 + 1 + PACKET_DATA_BYTES + 1

            if len(self.rx_buffer) < total_size:
                return

            raw = bytes(self.rx_buffer[:total_size])
            print("raww",raw)

            try:
                pkt = Packet.from_bytes(raw)

                del self.rx_buffer[:total_size]
                self._handle_packet(pkt)

            except Exception:
                # desync recovery
                self.rx_buffer.pop(0)

    def _handle_packet(self, pkt: Packet):
        print("\n[RX PACKET]")
        print(f"  length: {pkt.length}")
        print(f"  data:   {pkt.data[:pkt.length].decode(errors='ignore')}")
        print(f"  raw hex data: {pkt.data.hex(' ')}")
        print(f"  crc:    0x{pkt.crc:02X}")
    
        if pkt.is_ReTx():
            if self.last_packet:
                self.send_packet(self.last_packet)
        elif pkt.is_Ack():
            pass # Pode adicionar um evento aqui se necessário
        else:
            # Envia ACK automático se for um pacote de dados comum
            self.send_packet(Packet.single_byte_packet(PacketType.Ack_Packet.value))
            self.queue.put(pkt)

    def wait_packet(self, timeout: float = 5.0):
        try:
            return self.queue.get(timeout=timeout)
        except Empty:
            raise TimeoutError("timeout waiting for packet")

            
    
        
        