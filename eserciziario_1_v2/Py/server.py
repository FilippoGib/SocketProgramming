#!/usr/bin/env python3

import socket
import sys
import time

HOST = "127.0.0.1" #interfaccia di loopback
PORT = 2525 #hardcoded
FORMAT = "utf-8"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST,PORT))
    s.listen()
    conn, addr = s.accept()
    message = "Welcome from " + socket.gethostname()
    conn.sendall(message.encode(FORMAT))
    time.sleep(1)