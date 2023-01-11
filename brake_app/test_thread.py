# from PyQt6.QtCore import QThread, pyqtSignal

# class TestThread(QThread):
#     _status_signal = pyqtSignal(int)
#     def __init__(self) -> None:
#         super(TestThread, self).__init__()
#         self.period = None
#         self.path_to_file = None

#     def setPathToFile(self, path):
#         self.path_to_file = path

#     def setPeriod(self, period):
#         self.period = period

#     def __del__(self):
#         self.wait()

#     def run(self):
#         with open(self.path_to_file, "r") as file
