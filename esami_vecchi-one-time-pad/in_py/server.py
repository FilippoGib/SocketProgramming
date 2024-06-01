#!/usr/bin/env python3

import socket
import sys
import time
import threading

#alternativa 1: 
HOST = '127.0.0.1' #interfaccia standard di loopback

#alternativa 2:
#HOST = socket.gethostbyname(socket.gethostname())
PORT = 8080
FORMAT = 'utf-8'

input = sys.argv[1]

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST,PORT))
    s.listen()
    conn, addr = s.accept()
    conn.sendall(input.encode(FORMAT, errors = 'strict'))
    data = conn.recv(1024).decode(FORMAT, errors='strict')
    print(input)
    print('\n')
    print(data)
