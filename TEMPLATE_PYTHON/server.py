#!/usr/bin/env python3
import socket
import os
import sys
import argparse

FORMAT = 'utf-8'

def handle_client(conn, addr):
    #what you have to do
    print("Hello world")

def start(s, server):
    s.listen()
    print(f"[LISTENING] Server is listening on {server}")
    while True:
        conn, addr = s.accept()
        pid = os.fork()

        if pid == 0:
            handle_client(conn, addr)
            sys.exit()

        else:
            conn.close()
            os.wait()

def main():
    #leggi i commenti nel client
    parser = argparse.ArgumentParser(description='Server to handle clients')
    parser.add_argument('--server', type=str, default=socket.gethostbyname(socket.gethostname()), help='Server IP address')
    parser.add_argument('--port', type=int, default=8080, help='Server port number')
    args = parser.parse_args()

    ADDR = (args.server, args.port)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(ADDR)

    print("[STARTING] server is starting...")
    start(s, args.server)

if __name__ == "__main__":
    main()
