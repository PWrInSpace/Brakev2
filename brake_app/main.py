import sys
import qdarktheme
from pathlib import Path
from PyQt6.QtWidgets import QApplication
from PyQt6.QtWidgets import QLabel
from PyQt6.QtWidgets import QGridLayout
from PyQt6.QtWidgets import QWidget
from PyQt6.QtWidgets import QLineEdit
from PyQt6.QtWidgets import QPushButton
from PyQt6.QtWidgets import QFileDialog


class MainWindow(QWidget):

    def __init__(self):
        super(MainWindow, self).__init__()
        self.initializeUI()


    def initializeUI(self):
        layout = QGridLayout()
        self.setWindowTitle("QGridLayout")

        layout = QGridLayout()
        label = QLabel("File path")
        layout.addWidget(label)
        self.line_edit = QLineEdit()
        self.line_edit.setReadOnly(True)
        layout.addWidget(self.line_edit, 0, 1)
        button = QPushButton("Browse")
        button.clicked.connect(self.showDialog)
        layout.addWidget(button, 0, 2)
        self.setLayout(layout)

        self.setFixedWidth(500)
        self.setFixedHeight(400)

    def showDialog(self):
        home_dir = str(Path.home())
        fname = QFileDialog.getOpenFileName(directory=home_dir)
        if fname[0]:
            self.line_edit.clear()
            self.line_edit.insert(fname[0])

if __name__ == "__main__":
    app = QApplication(sys.argv)
    # qdarktheme.setup_theme()
    mw = MainWindow()
    mw.show()
    sys.exit(app.exec())