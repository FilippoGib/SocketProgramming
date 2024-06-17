#!/usr/bin/env python3
import socket
import os
import sys
import argparse
import re

FORMAT = 'utf-8'

def handle_client(conn, addr):
    #ricevo dati dalla connessione:
    data = conn.recv(128).decode(FORMAT)
    seed, iterations = data.split(",")
    patternSeed = r'^\d$'
    if not re.match(patternSeed, seed):
        conn.sendall("ERR, seed no match".encode(FORMAT))
        return
    
    iterations = iterations.split("\r\n")[0]
    patternIter = r'^\d+$'
    if not re.match(patternIter, iterations):
        conn.sendall("ERR, inter no match".encode(FORMAT))
        return
    
    conn.sendall(f"OK: {iterations} on seed {seed}\r\n".encode(FORMAT))
    
    i = 0
    string = ""
    while i < int(iterations):
        pattern = ""
        if i == 0:
            pattern = seed
        else:
            string = string.split('\r\n',1)[0]
            pattern = string
            string = ""
        visited = []
        for k in range (len(pattern)):
            counter = 0
            currentChar = pattern[k]
            if currentChar not in visited :
                visited.append(currentChar)
            else:
                continue
            for j in range (len(pattern)):
                if currentChar == pattern[j]:
                    counter += 1
            string = string + str(counter) + currentChar
        visited.clear()
        i+=1
        string = string + '\r\n'
        conn.sendall(string.encode(FORMAT))

def start(s, server):
    s.listen()
    print(f"[LISTENING] Server is listening...")
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
    parser.add_argument('--port', type=int, default=9090, help='Server port number')
    #add arguments if needed or override the default values
    args = parser.parse_args()

    ADDR = (args.server, args.port)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(ADDR)

        print(f"[STARTING] server is starting with server: {args.server}, on port: {args.port}")
        start(s, args.server)

if __name__ == "__main__":
    main()
