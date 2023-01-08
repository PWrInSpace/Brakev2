import sys
from PyQt6.QtWidgets import QApplication
from gui import MainWindow
from flight_data import FlightData


if __name__ == "__main__":
    app = QApplication(sys.argv)
    mw = MainWindow()
    fd = FlightData()
    mw.setCallbackLoadButtonClicked(fd.load_data_from_file)
    mw.show()
    sys.exit(app.exec())