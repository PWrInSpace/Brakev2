from pathlib import Path
from PyQt6.QtWidgets import QLabel
from PyQt6.QtWidgets import QGridLayout
from PyQt6.QtWidgets import QWidget
from PyQt6.QtWidgets import QLineEdit
from PyQt6.QtWidgets import QPushButton
from PyQt6.QtWidgets import QFileDialog


class MainWindow(QWidget):

    def __init__(self):
        super(MainWindow, self).__init__()
        self.path_to_file = None
        self.load_button = None
        self.load_button_cb = None
        self.status_text = None
        self.initializeUI()

    def initializeUI(self):
        self.setWindowTitle("Brejk")
        # self.setFixedWidth(500)
        # self.setFixedHeight(400)

        layout = QGridLayout()

        label = QLabel("File path")
        layout.addWidget(label, 0, 0)

        self.path_to_file = QLineEdit()
        self.path_to_file.setFixedWidth(200)
        self.path_to_file.setReadOnly(True)
        layout.addWidget(self.path_to_file, 0, 1)

        browse_button = QPushButton("Browse")
        browse_button.clicked.connect(self.showDialog)
        layout.addWidget(browse_button, 0, 2)

        self.load_button = QPushButton("Load")
        self.load_button.setEnabled(False)
        self.load_button.clicked.connect(self.loadButtonClicked)
        layout.addWidget(self.load_button, 0, 3)

        self.status_text = QLabel("Status: ")
        layout.addWidget(self.status_text, 1, 0, 1, 3)

        self.setLayout(layout)

    def updateStatus(self, text: str):
        self.status_text.setText(f"Status:  {text}")

    def setCallbackLoadButtonClicked(self, cb):
        self.load_button_cb = cb

    def loadButtonClicked(self):
        self.updateStatus("Loading data from file")
        if self.load_button_cb:
            self.load_button_cb(path=self.path_to_file.text())

        self.updateStatus("Preapared data created")


    def showDialog(self):
        home_dir = str(Path.home())
        fname = QFileDialog.getOpenFileName(directory=home_dir)
        if fname[0]:
            self.path_to_file.clear()
            self.path_to_file.insert(fname[0])
            self.load_button.setEnabled(True)
