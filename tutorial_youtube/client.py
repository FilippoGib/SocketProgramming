#!/usr/bin/env python3

import socket

PORT = 5050
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER, PORT)

c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
c.connect(ADDR)

def send(msg):
    message = msg.encode(FORMAT)
    c.send(message)
    print(c.recv(1024).decode(FORMAT))

send("Hello Man!\n")
input()
send("How are you Man?\n")
input()
send("Bomboclat Man\n")
input()
send(DISCONNECT_MESSAGE)
