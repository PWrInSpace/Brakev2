import sys
from PyQt6.QtWidgets import QApplication
from gui import MainWindow
import logging

if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    app = QApplication(sys.argv)
    mw = MainWindow()
    mw.show()
    sys.exit(app.exec())