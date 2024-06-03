#!/usr/bin/env python3
import socket
import sys
import os

HOST = '127.0.0.1' #standard loopback interface
PORT = 3333
FORMAT = 'utf-8'
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s: #standard for python
    s.bind((HOST,PORT))
    s.listen()
    while True :
        conn, addr = s.accept()
        pid = os.fork()
        if pid == 0 : #codice del figlio
            data  = conn.recv(1024).decode(FORMAT)
            print("Connessione ricevuta da: " + data)
        conn.close()