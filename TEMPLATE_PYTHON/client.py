#!/usr/bin/env python3
import socket
import os
import sys
import json
import argparse

FORMAT = 'utf-8'

def start(c):
   #what you have to do
   pass

def main():
    #parse serve a gestire gli input da linea di comando
    #es:
    #python client.py --server 192.168.1.2 --port 1234
    #python server.py --server 192.168.1.2 --port 1234

    parser = argparse.ArgumentParser(description='Client to connect to server')
    parser.add_argument('--server', type=str, default=socket.gethostbyname(socket.gethostname()), help='Server IP address')
    parser.add_argument('--port', type=int, default=8080, help='Server port number')
    #aggiungi arguments se necessario o sovrascrivi i valori di default
    args = parser.parse_args()

    ADDR = (args.server, args.port)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as c:
        c.connect(ADDR)

        print("[STARTING] client is starting...")
        start(c)

if __name__ == "__main__":
    main()
