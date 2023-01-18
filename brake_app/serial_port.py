import serial
import serial.tools.list_ports
import logging
import threading
import queue
import time


class CLI(threading.Thread):
    def __init__(self) -> None:
        super().__init__(daemon=True)
        self.serial = None
        self.read_cb = None
        self.write_queue = queue.Queue(10)

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

    def set_read_cb(self, cb):
        self.read_cb = cb

    def run(self):
        print("Running thread")
        message = ""
        while True:
            if self.write_queue.empty() is False:
                data = self.write_queue.get_nowait()
                self._write(data)
            if self.serial.in_waiting:
                try:
                    ret = self.serial.read().decode()
                    message += ret
                    if ret == '\n':
                        logging.info(message)
                        self.read_cb(rec_data=message)
                        message = ""
                except Exception as exc:
                    logging.error("Read error -> " + str(exc))

            time.sleep(0.0001)

    def connect(self, port = "/dev/ttyUSB1", baudrate = 115200, _timeout = 0.001) -> bool:
        try:
            self.serial = serial.Serial(port, baudrate, timeout=_timeout)
        except Exception as exc:
            logging.error("Serial port connect error :" + str(exc))
            return False

        return True

    def write(self, data:str):
        self.write_queue.put(data)

    def _write(self, data:str) -> int:
        if self.serial is None:
            logging.error("Serial port not open")
            return 0

        written_bytes = 0
        try:
            written_bytes = self.serial.write((data + "\r").encode())
            logging.debug("Written to serial:" + data)
        except Exception as exc:
            logging.error("Serial port write errror: " + str(exc))

        return written_bytes

    def read(self) -> str:
        if self.serial is None:
            logging.error("Serial port not open")
            return ""

        try:
            message = self.serial.read(100).decode()
        except Exception as exc:
            logging.error("Decode error" + str(exc))
            return ""

        for line in message.split("\n"):
            logging.info("Serial line: " + line)
            if line.startswith("\x1b[0;32mI") and "CLI" in line:
                logging.info("Line found:" + line)
                return line

        return ""

    def send_command(self, data:str) -> str:
        self.write(data)
        return self.read()

def read_cb(dupa):
    logging.info("INFO from dupa " + str(dupa))

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    ser = CLI()
    ser.connect(port = "/dev/ttyUSB1")
    ser.set_read_cb(read_cb)
    ser.start()
    while True:
        pass
    print(ser.send_command("sm-state"))