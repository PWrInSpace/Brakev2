from pathlib import Path
from PyQt6.QtWidgets import QLabel
from PyQt6.QtWidgets import QGridLayout
from PyQt6.QtWidgets import QWidget
from PyQt6.QtWidgets import QLineEdit
from PyQt6.QtWidgets import QPushButton
from PyQt6.QtWidgets import QFileDialog
from PyQt6.QtWidgets import QComboBox


class MainWindow(QWidget):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.path_to_file = None
        self.load_button = None
        self.load_button_cb = None
        self.refresh_button_cb = None
        self.connect_to_port_cb = None
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
        self.path_to_file.setReadOnly(True)
        # self.path_to_file.setFixedWidth(170)
        layout.addWidget(self.path_to_file, 0, 1, 1, 3)

        browse_button = QPushButton("Browse")
        browse_button.clicked.connect(self.showDialog)
        layout.addWidget(browse_button, 0, 4)

        self.load_button = QPushButton("Load")
        self.load_button.setEnabled(False)
        self.load_button.clicked.connect(self.loadButtonClicked)
        layout.addWidget(self.load_button, 0, 5)

        port_label = QLabel("Port")
        # port_label.all
        layout.addWidget(port_label, 1, 0)
        self.port_combo_box = QComboBox()
        layout.addWidget(self.port_combo_box, 1, 1)

        port_label = QLabel("Baudrate")
        layout.addWidget(port_label, 1, 2)
        self.baudrate_combo_box = QComboBox()
        layout.addWidget(self.baudrate_combo_box, 1, 3)

        self.connect_button = QPushButton("Connect")
        self.connect_button.clicked.connect(self.connectButtonClicked)
        layout.addWidget(self.connect_button, 1, 4)

        self.refresh_button = QPushButton("Refresh")
        self.refresh_button.clicked.connect(self.refreshButtonClicked)
        layout.addWidget(self.refresh_button, 1, 5)

        connection_text_label = QLabel("Connection status:")
        layout.addWidget(connection_text_label, 2, 0, 1, 2)
        self.connection_label = QLabel("Disconnected")
        layout.addWidget(self.connection_label, 2, 2, 1, 4)

        file_status_text_label = QLabel("File status:")
        layout.addWidget(file_status_text_label, 3, 0, 1, 2)
        self.file_status_label = QLabel("Not created")
        layout.addWidget(self.file_status_label, 3, 2, 1, 4)

        self.log_label = QLabel("Log: ")
        layout.addWidget(self.log_label, 4, 0, 1, 4)

        self.setLayout(layout)

    def updateLog(self, text: str):
        self.log_label.setText(f"Log:  {text}")

    def setCallbackLoadButtonClicked(self, cb):
        self.load_button_cb = cb

    def loadButtonClicked(self):
        self.updateLog("Loading data from file")
        if self.load_button_cb:
            if self.load_button_cb(path=self.path_to_file.text()):
                self.file_status_label.setText("Created")

        self.updateLog("Data ready for test")

    def setAvailablePorts(self, items:list):
        self.port_combo_box.addItems(items)

    def setBaudrates(self, items:list):
        self.baudrate_combo_box.addItems(items)

    def refreshButtonClicked(self):
        if self.refresh_button_cb:
            self.port_combo_box.clear()
            ports = self.refresh_button_cb()
            self.setAvailablePorts(ports)

    def callbackPortUpdate(self, cb):
        self.refresh_button_cb = cb

    def connectButtonClicked(self):
        if self.connect_to_port_cb:
            port = self.port_combo_box.currentText()
            baud = self.baudrate_combo_box.currentText()
            # self.connect_to_port_cb(port,int(baud))
            self.connection_label.setText("Connected")
            self.updateLog("Connected to serial port " + port)

    def callbackConnectToPort(self, cb):
        self.connect_to_port_cb = cb


    def showDialog(self):
        home_dir = str(Path.home())
        fname = QFileDialog.getOpenFileName(directory=home_dir)
        if fname[0]:
            self.path_to_file.clear()
            self.path_to_file.insert(fname[0])
            self.load_button.setEnabled(True)
