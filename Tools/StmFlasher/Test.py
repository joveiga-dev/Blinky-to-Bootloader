import serial
from Crc import Crc8_Compute
import time
PORT = 'COM12'
BAUDRATE = 9600

ser = serial.Serial(PORT, BAUDRATE, timeout=1)
print(f"Listening at {PORT} @ {BAUDRATE}\n")

PACKET_SIZE = 18

def unpack(raw_data):
    if len(raw_data) != PACKET_SIZE:
        return None

    length = raw_data[0]
    data = raw_data[1:17]   # 16 bytes
    crc = raw_data[17]

    return {
        "length": length,
        "data": list(data),
        "crc": crc
    }

buffer = bytearray()

def log(prefix, data):
    print(f"[{prefix}] {data.hex(' ')}")

def build_packet(data: bytes):
    length = len(data)
    payload = bytes([length]) + data
    crc = Crc8_Compute(payload)
    return payload + bytes([crc])

def send_packet(data: bytes):
    pkt = build_packet(data)
    ser.write(pkt)
    log("TX", pkt)

while True: 
    buffer += ser.read(64) # Build packets from buffer 
    while len(buffer) >= PACKET_SIZE:
        packet_bytes = buffer[:PACKET_SIZE] 
        buffer = buffer[PACKET_SIZE:] 
        packet = unpack(packet_bytes) 
        print(packet_bytes.hex(' '))
        print(f"Packet Received: {packet}") 
        time.sleep(1)
    
    
        