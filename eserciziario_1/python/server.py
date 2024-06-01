#!/usr/bin/env python3
import socket
import sys
import time

HOST = '127.0.0.1' #standard loopback interface
PORT = 2525
FORMAT = 'utf-8'
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s: #standard for python
    s.bind((HOST,PORT))
    s.listen()
    conn, addr= s.accept()
    conn.sendall(socket.gethostname().encode(FORMAT))
    time.sleep(1)