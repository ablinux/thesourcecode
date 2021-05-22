
from EncoderTest import Ui_MainWindow
from PyQt5.QtGui import  QColor
from PyQt5 import QtGui, QtWidgets
import socket
class EmbeddedTestWorker(QtWidgets.QMainWindow,Ui_MainWindow):
    '''Ui setup'''
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setupUi(self)

        self.pushButton_send.clicked.connect(sendUDPMsg)


def sendUDPMsg():
    print("Sending")
    msgFromClient = "SA1600P1100L180#"

    bytesToSend = str.encode(msgFromClient)

    serverAddressPort = ("192.168.0.207", 1122)

    bufferSize = 1024

    # Create a UDP socket at client side

    UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

    # Send to server using created UDP socket

    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    pass

if __name__ == '__main__':
    global widget,RUN
    app = QtWidgets.QApplication([])
    # app.setStyle('Fusion')
    # app.setStyleSheet(qdarkstyle.load_stylesheet())
    # app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())
    widget = EmbeddedTestWorker()
    widget.show()
    app.exec()



