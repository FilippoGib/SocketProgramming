#!/usr/bin/env python3
import socket

HOST='localhost'
PORT= 1025

def send_request(sock, req):
    print(f'Sending request {req}')
    sock.sendall(req)

def check_response(sock, resp):
    recv_resp=sock.recv(6)
    if recv_resp ==resp:
        print(f'Ok:Received response {recv_resp}')
        return True
    else:
        print(f'ERROR: Expecting {resp}, got {recv_resp}')
        return False
    


data=[(b'Q\x0a127.0.0.1\x00',   b'R\x04\x7f\x00\x00\x01'),
      (b'Q\x0910.1.1.1\x00',    b'R\x04\x0a\x01\x01\x01'),
      (b'Q\x0c192.168.1.1\x00', b'R\x04\xc0\xa8\x01\x01')]

if __name__ == '__main__':
    for req, resp in data:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        send_request(sock, req)
        rv=check_response(sock, resp)
        sock.close()
        if not rv: break
