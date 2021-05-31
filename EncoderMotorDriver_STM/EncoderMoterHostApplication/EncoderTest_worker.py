from PyQt5.QtWidgets import QFileDialog
import turtle as tt
from EncoderTest import Ui_MainWindow
from PyQt5.QtGui import  QColor
from PyQt5 import QtGui, QtWidgets
import socket

# define command index to easy parsing
START = 0
RUN_TYPE = 1
PORT = 6
DIR = 11

CoordItems = []
ENDOFCMD = "EOC@"
class EmbeddedTestWorker(QtWidgets.QMainWindow,Ui_MainWindow):
    '''Ui setup'''
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setupUi(self)

        self.pushButton_send.clicked.connect(sendUDPMsg)
        self.pushButton_Add_Seq.clicked.connect(AddSeq2List)
        self.actionAdd_csf_file.triggered.connect(load_csf_file)
        self.actionSave_coordinate_secquence.triggered.connect(save_csf_file)
        self.pushButton_Play.clicked.connect(play)
        self.pushButton_cleanList.clicked.connect(clearList)
        self.pushButton_delete_item.clicked.connect(deleteItem)

def clearList():
    widget.listWidget_coordList.clear()
    CoordItems.clear()
    pass

def deleteItem():
    index = widget.listWidget_coordList.currentRow()
    del CoordItems[index]
    removeSel()
    pass

def removeSel():
    listItems=widget.listWidget_coordList.selectedItems()
    if not listItems: return
    for item in listItems:
       widget.listWidget_coordList.takeItem(widget.listWidget_coordList.row(item))

def sendUDPMsg():
    serverAddressPort = ("192.168.0.207", 1122)

    # Send to server using created UDP socket
    # Create a UDP socket at client side
    UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    command = "S{}{}P{}{}{}#@".format(move_type(),widget.lineEdit_distance.text(),linear_dir(),angular_dir(),widget.lineEdit_direction.text())
    bytesToSend = str.encode(command)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    pass

def play():
    serverAddressPort = ("192.168.0.207", 1122)

    # Send to server using created UDP socket
    # Create a UDP socket at client side
    UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    index = 0
    # Send to server using created UDP socket
    for Run_dir in CoordItems:
        bytesToSend = str.encode(Run_dir)
        UDPClientSocket.sendto(bytesToSend, serverAddressPort)
        # Highlight the current executing item from listWidget
        widget.listWidget_coordList.hasFocus()
        # draw_path(Run_dir, 2)
        fromCN1 = UDPClientSocket.recvfrom(5)
        print(fromCN1)

    bytesToSend = str.encode(ENDOFCMD)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    pass

def AddSeq2List():
    global  CoordItems
    # Create the coordinate cmds
    command = "S{}{}P{}{}{}#@".format(move_type(),widget.lineEdit_distance.text(),linear_dir(),angular_dir(),widget.lineEdit_direction.text())
    CoordItems.append(command)
    widget.listWidget_coordList.addItem(command)
    pass

def move_type():
    if widget.radioButton_Distance.isChecked() is True:
        return "D"
    if widget.radioButton_Ticks.isChecked() is True:
        return "T"
    if widget.radioButton_angular.isChecked() is True:
        return "A"

def angular_dir():
    if widget.radioButton_Left.isChecked() is True:
        return 'L'
    if widget.radioButton_C_Left.isChecked() is True:
        return 'l'
    if widget.radioButton_Right.isChecked() is True:
        return 'R'
    if widget.radioButton_C_Right.isChecked() is True:
        return 'r'
def linear_dir():
    if widget.radioButton_FWD.isChecked() is True:
        return '1100'
    if widget.radioButton_BACK.isChecked() is True:
        return '0011'

def load_csf_file():
    print("Load CSF file")
    global widget
    name,type = QFileDialog.getOpenFileNames(widget, 'Load File')
    print(name[0])
    if name[0] is None:
        return
    file = open(name[0], 'r')
    CoordItems.clear()
    widget.listWidget_coordList.clear()
    coordinates=file.readline()
    while coordinates is not '':
        coordinates = coordinates[:-1]
        CoordItems.append(coordinates)
        widget.listWidget_coordList.addItem(coordinates)
        coordinates = file.readline()
    print(CoordItems)
    s = "File saved at : {}".format(name[0] + '.csf')
    print(s)
    pass

def save_csf_file():
    global widget
    name = QFileDialog.getSaveFileName(widget, 'Save File')
    if name[0] is None:
        return
    file = open(name[0]+'.csf', 'w+')
    for coord in CoordItems:
        file.write(coord+'\n')
    s = "File saved at : {}".format(name[0]+'.csf')
    print(s)
    pass

def split(word):
    return [char for char in word]

def draw_path(coord_cmd,scale):
    # lets parse what command has to say
    cmd = split(coord_cmd)
    if(cmd[RUN_TYPE]=='A'):
        dir = cmd[DIR]
        dir_attr = cmd[DIR+1:DIR+4] # get the angle value
        dir_attr = int(list_to_str(dir_attr))
        print ("{}  {}".format(dir,dir_attr))
        if(dir == 'L' or dir == 'l'):
            tt.left(dir_attr)
        if (dir == 'R' or dir == 'r'):
            tt.right(dir_attr)
    if(cmd[RUN_TYPE]=='D'):
        dist_attr = cmd[RUN_TYPE+1:RUN_TYPE+5]
        dist_attr = int(list_to_str(dist_attr))
        # also need to know FWD or BACK
        dir_mov = cmd[PORT+1:PORT+5]
        dir_mov = list_to_str(dir_mov)
        if(dir_mov == "1100"): # FWD
            tt.forward(dist_attr/scale)
        if(dir_mov == "0011"):
            tt.back(dir_attr/scale)
    pass

def list_to_str(s):
    # initialization of string to ""
    new = ""

    # traverse in the string
    for x in s:
        new += x

        # return string
    return new

if __name__ == '__main__':
    global widget,RUN
    app = QtWidgets.QApplication([])
    # app.setStyle('Fusion')
    # app.setStyleSheet(qdarkstyle.load_stylesheet())
    # app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())
    widget = EmbeddedTestWorker()
    widget.lineEdit_direction.hide()
    widget.groupBox_4.hide()
    widget.label.hide()
    widget.show()
    app.exec()



