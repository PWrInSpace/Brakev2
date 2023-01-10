import serial
import serial.tools.list_ports
import logging
import re

class CLI():
    def __init__(self):
        self.serial = None

    @staticmethod
    def get_port_list() -> list:
        ports = serial.tools.list_ports.comports()
        ports_str = []
        for port, desc, hwid in sorted(ports):
            ports_str.append(str(port))

        return ports_str

    @staticmethod
    def get_baudrate_list() -> list:
        return ["9600", "115200"]

    def connect(self, port = "/dev/ttyUSB1", baudrate = 115200, _timeout = 1) -> bool:
        try:
            self.serial = serial.Serial(port, baudrate, timeout=_timeout)
        except Exception as exc:
            logging.error("Serial port connect error :" + str(exc))
            return False

        return True

    def write(self, data:str) -> int:
        if self.serial is None:
            logging.error("Serial port not open")
            return 0

        written_bytes = 0
        try:
            written_bytes = self.serial.write((data + "\r\n").encode())
            logging.debug("Written to serial:" + data)
        except Exception as exc:
            logging.error("Serial port write errror: " + str(exc))

        return written_bytes

    def read(self) -> str:
        if self.serial is None:
            logging.error("Serial port not open")
            return ""

        message = self.serial.read(1000).decode()
        for line in message.split("\n"):
            if line.startswith("\x1b[0;32mI") and "CLI" in line:
                logging.info("Line found:" + line)
                return line

        return ""

    def send_command(self, data:str) -> str:
        self.write(data)
        return self.read()

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    ser = CLI()
    ser.connect(port = "/dev/ttyUSB0")
    print(ser.send_command("sm-state"))