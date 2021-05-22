import socket
import time
# Sq run test
PATH = ["SD0600P1100L180#@","SA1600P1100L090#@","SD0600P1100L180#@","SA1600P1100L090#@","SD0600P1100L180#@","SA1600P1100L090#@","SD0600P1100L180#@"]
ENDOFCMD = "EOC@"
msgFromClient = "SA1600P1100L180#"

bytesToSend = str.encode(msgFromClient)

serverAddressPort = ("192.168.0.207", 1122)

bufferSize = 1024

# Create a UDP socket at client side

UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
# UDPClientSocket.bind("192.168.0.207", 1122)
# Send to server using created UDP socket
for Run_dir in PATH:
    bytesToSend = str.encode(Run_dir)
    # fromCN1 = UDPClientSocket.recvfrom(bufferSize)
    # print(fromCN1)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)

bytesToSend = str.encode(ENDOFCMD)
UDPClientSocket.sendto(bytesToSend, serverAddressPort)
