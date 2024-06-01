#!/usr/bin/env python3

import socket
import threading

PORT = 5050
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT" #when we receive this message we gracefully disconnect

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(ADDR)

def handle_client(conn, addr):

    print(f"[NEW CONNECTION] {addr} connected")

    connected = True

    while connected:
        data = conn.recv(1024).decode(FORMAT)
        if data: #only if we have actually sent something
            if data == DISCONNECT_MESSAGE:
                connected = False

            print(f"[{addr}] sent: {data}")
            conn.send(f"I got your message: {data}".encode(FORMAT))
    
    conn.close()


def start():
    s.listen()
    print(f"[LISTENING] Server is listening on {SERVER}")
    while True:
        conn, addr = s.accept()
        thread = threading.Thread(target=handle_client, args=(conn,addr))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.active_count() -1}") #just prints the number of threads active at this moment - 1 which is the "start" thread

print("[STARTING] server is starting...")
start()