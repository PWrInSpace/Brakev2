import time
from pathlib import Path
from PyQt6.QtWidgets import QLabel
from PyQt6.QtWidgets import QGridLayout
from PyQt6.QtWidgets import QWidget
from PyQt6.QtWidgets import QLineEdit
from PyQt6.QtWidgets import QPushButton
from PyQt6.QtWidgets import QFileDialog
from PyQt6.QtWidgets import QComboBox
from PyQt6.QtWidgets import QProgressBar
from PyQt6.QtGui import QCloseEvent
from serial_port import CLI
from flight_data import FlightData
import threading


class MainWindow(QWidget):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.initializeUI()
        self.cli = CLI()
        self.flight_data = FlightData()
        self.test_data_timer = None

    def initializeUI(self):
        self.setWindowTitle("Brejk")
        layout = QGridLayout()

        self._layoutFile(layout)
        self._layoutSerialPort(layout)
        self._layoutTestMode(layout)
        self._layoutStatus(layout)
        self._layoutLog(layout)
        self.setLayout(layout)
        self.initSerialInfo()

    def _layoutFile(self, layout):
        label = QLabel("File path")
        layout.addWidget(label, 0, 0)

        self.path_to_file = QLineEdit()
        self.path_to_file.setReadOnly(True)
        layout.addWidget(self.path_to_file, 0, 1, 1, 3)

        browse_button = QPushButton("Browse")
        browse_button.clicked.connect(self.showDialog)
        layout.addWidget(browse_button, 0, 4)

        self.load_button = QPushButton("Load")
        self.load_button.setEnabled(False)
        self.load_button.clicked.connect(self.loadButtonClicked)
        layout.addWidget(self.load_button, 0, 5)

    def _layoutSerialPort(self, layout):
        port_label = QLabel("Port")
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

    def _layoutTestMode(self, layout):
        test_mode_label = QLabel("Test mode cmd")
        layout.addWidget(test_mode_label, 2, 0)
        self.button_test_mode_cmd = QPushButton("Send")
        self.button_test_mode_cmd.setDisabled(True)
        self.button_test_mode_cmd.clicked.connect(self.testModeCmdButtonClicked)
        layout.addWidget(self.button_test_mode_cmd, 2, 1)

        test_label = QLabel("Test")
        layout.addWidget(test_label, 2, 2)
        self.button_test = QPushButton("Start")
        self.button_test.setDisabled(True)
        self.button_test.clicked.connect(self.testModeStartButtonClicked)
        layout.addWidget(self.button_test, 2, 3)

        progress_label = QLabel("Test progress")
        layout.addWidget(progress_label, 3, 0)
        self.progress = QProgressBar()
        # self.progress.setGeometry(200, 80, 250, 20)
        layout.addWidget(self.progress, 3, 1, 1, 5)


    def _layoutStatus(self, layout):
        connection_text_label = QLabel("Connection status:")
        layout.addWidget(connection_text_label, 4, 0, 1, 2)
        self.connection_label = QLabel("Disconnected")
        layout.addWidget(self.connection_label, 4, 2, 1, 4)

        file_status_text_label = QLabel("File status:")
        layout.addWidget(file_status_text_label, 5, 0, 1, 2)
        self.file_status_label = QLabel("Not created")
        layout.addWidget(self.file_status_label, 5, 2, 1, 4)

    def _layoutLog(self, layout):
        self.log_label = QLabel("Log: ")
        layout.addWidget(self.log_label, 6, 0, 1, 4)

    def initSerialInfo(self):
        self.port_combo_box.addItems(CLI.get_port_list())
        self.baudrate_combo_box.addItems(CLI.get_baudrate_list())

    def updateLog(self, text: str):
        self.log_label.setText(f"Log:  {text}")

    def loadButtonClicked(self):
        self.updateLog("Loading data from file")
        if self.flight_data.load_data(self.path_to_file.text()):
            self.file_status_label.setText("Created")
            self.updateLog("Data ready for test")
        else:
            self.updateLog("Unable to load data")

    def refreshButtonClicked(self):
        self.port_combo_box.clear()
        self.port_combo_box.addItems(CLI.get_port_list())

    def connectButtonClicked(self):
        port = self.port_combo_box.currentText()
        baud = self.baudrate_combo_box.currentText()
        if self.cli.connect(port, int(baud), 1):
            self.connection_label.setText("Connected")
            self.updateLog("Connected to serial port " + port)
            self.button_test_mode_cmd.setDisabled(False)
        else:
            self.updateLog("Unable to connect to serial port " + port)

    def testModeCmdButtonClicked(self):
        ret = self.cli.send_command("test-mode")
        if "CLI" in ret:
            time.sleep(0.5)
            self.updateLog("Test mode turned on")
            self.button_test.setDisabled(False)

    def testModeCallback(self):
        data = self.flight_data.getPreparedLine()
        pos = self.flight_data.getPreapredDataPosition()
        data_len = self.flight_data.getPreparedDataLength()
        self.cli.send_command(data)
        self.progress.setValue(pos)
        if pos < data_len:
            self.test_data_timer = threading.Timer(0.01, self.testModeCallback)
            self.test_data_timer.start()
        else:
            self.button_test.setEnabled(True)

    def testModeStartButtonClicked(self):
        self.progress.setMinimum(0)
        self.progress.setMaximum(self.flight_data.getPreparedDataLength())
        self.progress.setValue(0)
        self.button_test.setEnabled(False)
        self.testModeCallback()

    def showDialog(self):
        home_dir = str(Path.home())
        fname = QFileDialog.getOpenFileName(directory=home_dir)
        if fname[0]:
            self.path_to_file.clear()
            self.path_to_file.insert(fname[0])
            self.load_button.setEnabled(True)

    def closeEvent(self, a0: QCloseEvent) -> None:
        if self.test_data_timer:
            self.test_data_timer.cancel()

        self.cli.send_command("exit")
        return super().closeEvent(a0)
