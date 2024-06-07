#!/usr/bin/env python3
import socket
import os
import sys
import json

PORT = 8080 #hardcoded
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(ADDR)

def handle_client(conn, addr):
    message = json.loads(conn.recv(1024).decode(FORMAT))
    file_name = message["filename"]

    with open(file_name, "rb") as f:
        file_content = f.read()
        filesize = len(file_content)
        response_json = {
            "filename": file_name,
            "filesize": filesize
        }
        response = json.dumps(response_json).encode() + b"\n" + file_content
        conn.sendall(response)

def start():
    s.listen()
    print(f"[LISTENING] Server is listening on {SERVER}")
    while True:
        conn, addr = s.accept()
        pid = os.fork()

        if pid == 0:
            handle_client(conn, addr)
            sys.exit()

        else:
            conn.close()

def main():

    print("[STARTING] server is starting...")
    start()
    

if __name__ == "__main__":
    main()


