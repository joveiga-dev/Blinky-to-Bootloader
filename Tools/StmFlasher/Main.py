from Uart import UartComms
from Packet import Packet
from time import sleep

ser = UartComms(timeout=2).connect(port="COM12", baudrate=9600)
rx_buffer = bytearray()
packets = []
last_packet = Packet.single_byte_packet(0xFF)

def write_packet(packet):
    global last_packet

    data = packet.to_bytes()
    ser.write(data)

    last_packet = packet
    # Debug
    print(f"[TX] {data.hex(' ')}")
