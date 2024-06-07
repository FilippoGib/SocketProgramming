#!/usr/bin/env python3
import socket
import os
import sys
import json
import argparse
import time

FORMAT = 'utf-8'

def start(c):
    filename = sys.argv[-1]
    print("[INFO] filename is: "+filename)
    content = b""
    header_json = b""
    with open(filename, "rb") as f:
        content = f.read()

    header_json = ({
        "filename": filename,
        "filesize": len(content)
    })

    message = json.dumps(header_json).encode(FORMAT) + b"\n" + content
    c.sendall(message)
    print("[INFO] File content sent to the server.")

    # Wait for response from the server
    response = c.recv(1024).decode(FORMAT)
    json_response = json.loads(response)
    code = json_response["statuscode"]
    print("status code: " + code + "\n")
    c.close()
    print("[INFO] Connection closed.")

def main():
    #parse serve a gestire gli input da linea di comando
    #modifica la porta di default se necessario
    #es:
    #python client.py --server 192.168.1.2 --port 1234
    #python server.py --server 192.168.1.2 --port 1234

    parser = argparse.ArgumentParser(description='Client to connect to server')
    parser.add_argument('--server', type=str, default=socket.gethostbyname(socket.gethostname()), help='Server IP address')
    parser.add_argument('--port', type=int, default=5002, help='Server port number')
    
    args, unknown = parser.parse_known_args()

    ADDR = (args.server, args.port)

    c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    c.connect(ADDR)

    print("[STARTING] client is starting...")
    start(c)

if __name__ == "__main__":
    main()
