import sys
from PyQt6.QtWidgets import QApplication
from gui import MainWindow
from flight_data import FlightData
from serial_port import CLI


if __name__ == "__main__":
    app = QApplication(sys.argv)
    mw = MainWindow()
    mw.show()
    sys.exit(app.exec())