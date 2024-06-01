import socket
import json
import os

def handle_client_connection(client_socket):
    request = client_socket.recv(1024)
    request_json = json.loads(request.decode())
    
    filename = request_json["filename"]
    
    try:
        with open(filename, "rb") as f:
            file_content = f.read()
            filesize = len(file_content)
            response_json = {
                "filename": filename,
                "filesize": filesize
            }
            response = json.dumps(response_json).encode() + b"\n" + file_content
            client_socket.sendall(response)
    except FileNotFoundError:
        response_json = {
            "error": "File not found"
        }
        response = json.dumps(response_json).encode()
        client_socket.sendall(response)
    
    client_socket.close()

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("127.0.0.1", 8080))  # Ascolta solo su localhost
    server.listen(5)
    print("Server listening on port 8080")
    
    while True:
        client_socket, addr = server.accept()
        print(f"Accepted connection from {addr}")
        handle_client_connection(client_socket)

if __name__ == "__main__":
    start_server()
