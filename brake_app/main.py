import sys
from PyQt6.QtWidgets import QApplication
from gui import MainWindow
from flight_data import FlightData
from serial_port import CLI


if __name__ == "__main__":
    app = QApplication(sys.argv)
    mw = MainWindow()
    fd = FlightData()
    cli = CLI()
    mw.setCallbackLoadButtonClicked(fd.qtcb_load_data_from_file)
    mw.setBaudrates(CLI.get_baudrate_list())
    mw.setAvailablePorts(CLI.get_port_list())
    mw.callbackPortUpdate(CLI.get_port_list)
    mw.callbackConnectToPort(CLI.connect)
    mw.show()
    sys.exit(app.exec())