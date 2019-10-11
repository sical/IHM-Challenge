#!/usr/bin/env python3
#Michael ORTEGA - 09 jan 2018

###############################################################################
## Global libs
import socket
import sys
import select
from time import sleep

address = ('localhost', 6006)
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sleep(4)
data = b'P_UP'
print('sending')
client_socket.sendto(data, address)




sleep(3)
data = b'STOP'
client_socket.sendto(data, address)

