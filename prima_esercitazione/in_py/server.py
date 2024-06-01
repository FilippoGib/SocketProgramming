#!/usr/bin/env python3

import socket
import sys
import time
import threading

#alternativa 1: 
#HOST = '127.0.0.1' #interfaccia standard di loopback

#alternativa 2:
HOST = socket.gethostbyname(socket.gethostname())
PORT = int(sys.argv[1])

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST,PORT))
    s.listen()
    conn, addr = s.accept()
    print('Connected by', addr)
    data = conn.recv(1024)
    print('Here is message: %s'% data.decode('utf-8'))
    conn.sendall('I got your message'.encode('utf-8'))
    time.sleep(1)
