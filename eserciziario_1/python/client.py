#!/usr/bin/env python3
import socket
import sys

HOST = sys.argv[1]
PORT = 2525
FORMAT = 'utf-8'

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST,PORT))
    data = s.recv(1024)
    s.close
print(f"hostname is: {data.decode(FORMAT)}")