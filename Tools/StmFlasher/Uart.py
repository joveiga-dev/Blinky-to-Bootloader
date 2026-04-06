from serial import Serial, SerialException
import serial.tools.list_ports

from Const import baudRate, DataBits, StopBits, Parity

class UartComms:
    def __init__(self, timeout = 1):
        self.ser = None
        self.timeout = timeout
    
    def connect(self, port: str, baudrate: int = baudRate):
        try:
            self.ser = Serial(
                port=port,
                baudrate=baudrate,
                bytesize=DataBits,
                parity=Parity,
                stopbits=StopBits,
                timeout = self.timeout
            )
            print(f"Success connected to {port} at {baudrate}")
            return True
        except SerialException as e:
            print(f"Error connecting to {port} at {baudrate} baud: {e}")
            return False
        
    def disconnected(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
            return True
        raise ConnectionError("No serial port is not connected.")
    
    def refresh_ports(self):
        """
        Refreshes the list of available serial ports
        returns list of these ports
        """
        all_ports = serial.tools.list_ports.comports()
        return [port.device for port in all_ports]
    
    def send_packet(self, packet):
        if self.ser and self.ser.is_open:
            self.ser.write(packet)
        else:
            raise ConnectionError("Cannot send packet: No serial port connected.")
    
        
    
    
    
    

