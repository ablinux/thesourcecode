import socket
import turtle as tt

angle = '83'
dir = ''

#Just move FWD
# PATH = ["SA0695P1100l090#@"]
# Sq run test
#BOX
# PATH = ["SD0600P1100L180#@","SA1600P1100L090#@","SD0600P1100L180#@","SA1600P1100L090#@","SD0600P1100L180#@","SA1600P1100L090#@","SD0600P1100L180#@","SA1600P1100L090#@"]
# Box With Robo center rotation
PATH = ['SD0600P1100l'+angle+'#@','SA0600P1100l'+angle+'#@','SD0600P1100l'+angle+'#@','SA0600P1100l'+angle+'#@','SD0600P1100l'+angle+'#@','SA0600P1100l'+angle+'#@','SD0600P1100l'+angle+'#@','SA0600P1100l'+angle+'#@']
# Dance
# PATH = ["ST0060P0011R090#@","ST0060P1100R090#@","ST0060P0011R090#@","ST0060P1100R090#@","ST0060P0011R090#@","ST0060P1100R090#@","ST0060P0011R090#@","ST0060P1100R090#@",]
ENDOFCMD = "EOC@"

msgFromClient = "SA1600P1100L180#"
bytesToSend = str.encode(msgFromClient)
serverAddressPort = ("192.168.0.207", 1122)
bufferSize = 1024
# define command index to easy parsing
START = 0
RUN_TYPE = 1
PORT = 6
DIR = 11


def list_to_str(s):
    # initialization of string to ""
    new = ""

    # traverse in the string
    for x in s:
        new += x

        # return string
    return new

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
        if(dir == 'L'):
            tt.left(dir_attr)
        if (dir == 'R'):
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

# Create a UDP socket at client side

UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# Send to server using created UDP socket
for Run_dir in PATH:
    bytesToSend = str.encode(Run_dir)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    # draw_path(Run_dir, 2)
    fromCN1 = UDPClientSocket.recvfrom(5)
    print(fromCN1)

bytesToSend = str.encode(ENDOFCMD)
UDPClientSocket.sendto(bytesToSend, serverAddressPort)

tt.done

