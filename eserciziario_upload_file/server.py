#!/usr/bin/env python3
import socket
import os
import sys
import argparse
import json
import time

FORMAT = 'utf-8'

def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    message = b""
    
    while True:
        part = conn.recv(128) #ho provato a cambiare questa dimensione ma non cambia nulla
        if not part:
            break
        message += part

    try:
        header_end = message.index(b"}") + 1
        header = message[:header_end]
        content = message[header_end:]
        header_json = json.loads(header.decode(FORMAT))
        print(f"[HEADER] Received header: {header_json}")
    except (ValueError, json.JSONDecodeError) as e:
        print(f"[ERROR] Incorrect header format: {e}")
        conn.close()
        return

    filename = header_json["filename"]
    name, ext = filename.rsplit(".", 1)
    new_filename = f"{name}_uploaded.{ext}"

    with open(new_filename, "wb") as f:
        f.write(content)
    print(f"[SUCCESS] File {new_filename} written successfully.")

    response = json.dumps({"statuscode": "200"}).encode(FORMAT)
    conn.sendall(response)
    print(f"[RESPONSE] Response sent to {addr}.")

    conn.close()
    print(f"[DISCONNECT] {addr} disconnected.")

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
    parser = argparse.ArgumentParser(description='Server to handle clients')
    parser.add_argument('--server', type=str, default=socket.gethostbyname(socket.gethostname()), help='Server IP address')
    parser.add_argument('--port', type=int, default=5002, help='Server port number')
    args = parser.parse_args()

    ADDR = (args.server, args.port)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(ADDR)

    print("[STARTING] server is starting...")
    start(s, args.server)

if __name__ == "__main__":
    main()
