#!/usr/bin/env python3

import socket
import os
import sys

HOST = "127.0.0.1" #interfaccia di loopback
PORT = 2522 #hardcoded
FORMAT = "utf-8"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:

    s.bind((HOST,PORT))
    s.listen()

    while True:
        conn, addr = s.accept()
        pid = os.fork()
        if pid == 0 :
            message = "Welcome from " + socket.gethostname()
            conn.sendall(message.encode(FORMAT))
            sys.exit()
        conn.close()