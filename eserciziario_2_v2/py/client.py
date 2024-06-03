#!/usr/bin/env python3
import socket
import sys

HOST = sys.argv[1]
PORT = 3333
FORMAT = 'utf-8'

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST,PORT))
    data = s.sendall(socket.gethostname().encode(FORMAT))
    s.close
