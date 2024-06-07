#!/usr/bin/env python3
import socket
import os
import sys
import json

PORT = 8080
FORMAT = 'utf-8'
SERVER = socket.gethostbyname(socket.gethostname())
ADDR = (SERVER, PORT)

c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
c.connect(ADDR)

def start():
    filename = sys.argv[1] #only command line argument is the name of the file to download
    message = {
    "filename": filename
    }

    
    c.sendall(json.dumps(message).encode(FORMAT))

    received = b""

    while True:
        part = c.recv(1024)
        if not part:
            break
        received += part

    header, content = received.split(b"}",1)
    name, extension = filename.split(".",1)

    with open(name + "_downloaded." + extension, "wb") as f:
        f.write(content)

    c.close()

def main():

    print("[STARTING] client is starting...")
    start()
    

if __name__ == "__main__":
    main()
