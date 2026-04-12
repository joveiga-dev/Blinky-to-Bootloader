from serial import Serial, SerialException
from serial.tools import list_ports
from typing import Optional

class UartComms:
    """
    Low-level Uart Communication Handler
        It provides a minimal abstraction over a serial port. Its only reponsability is for raw
        data transport and connection management.
    """
    
    def __init__(self, timeout:float = 1):
        self.ser: Optional[Serial] = None
        self.timeout = timeout
    
    def connect(self, port: str, baudrate: int = 9600)->bool:
        """
        Open a serial connection
        Args:
            port (str): Serial port path
            baudrate (int, optional): Comms speed in baud. Defaults to baudRate.

        Returns:
            bool: True if connection succeeds, False otherwise
        """
        if self.ser and self.ser.is_open:
            print("Already conmnected")
            return True
        try:
            self.ser = Serial(
                port=port,
                baudrate=baudrate,
                timeout = self.timeout
            )
            print(f"Success connected to {port} at {baudrate}")
            return True
        except SerialException as e:
            print(f"Error connecting to {port} at {baudrate} baud: {e}")
            return False
        
    def disconnect(self)->None:
        """
        Close the serial connection
        Raises:
            ConnectionError: If no active connection found
        """
        
        if self.ser and self.ser.is_open:
            self.ser.close()
            print(f"Disconnected")
        else:
            raise ConnectionError("serial port is not connected.")
    
    def send_data(self, data: bytes)->None:
        """
        Sends raw bytes over serial connection
        Args: data (bytes): Data to transmit.
        """
        
        if not self.ser or not self.ser.is_open:
            raise ConnectionError("Cannot send Data: No serial port connected.")
        self.ser.write(data)
        
    def receive_data(self, timeout=10) ->bytes:
        """
        Read raw bytes from the serial port

        """
        if not self.ser or not self.ser.is_open:
            raise ConnectionError("Cannot receive Data: No serial port connected.")
        import time
        timeout = time.time() + timeout
        while time.time() < timeout:
            if self.ser.in_waiting > 0:
                data = self.ser.read(self.ser.in_waiting)
                return data
        if not data:
            raise TimeoutError("No data received within the specified timeout.")
        return data
    
    def flush(self)->None:
        """
        Clear Input/Output Buffers
        """
        if self.ser and self.ser.is_open:
            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()

    def refresh_ports(self) ->list[str]:
        """
        Get list of available serial ports
        Returns:
            list[str] of these ports
        """
        return [port.device for port in list_ports.comports()]
            
        
            
    
        
    
    
    
    

