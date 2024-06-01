import socket
import json
import argparse

def download_file(server_address, port, filename, save_as):
    # Crea la richiesta JSON
    request_json = {
        "filename": filename
    }
    request = json.dumps(request_json).encode()
    
    # Connessione al server
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((server_address, port))
    client.sendall(request)
    
    # Ricezione della risposta
    response = b""
    while True:
        part = client.recv(1024)
        if not part:
            break
        response += part
    
    # Separazione dell'header e del contenuto del file
    header, file_content = response.split(b"\n", 1)
    response_json = json.loads(header.decode())
    
    # Gestione della risposta
    if "error" in response_json:
        print("Error:", response_json["error"])
    else:
        with open(save_as, "wb") as f:
            f.write(file_content)
        print(f"File {response_json['filename']} downloaded successfully as {save_as}")
    
    client.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Download a file from the server")
    parser.add_argument("filename", help="The name of the file to download")
    parser.add_argument("save_as", help="The name to save the downloaded file as")
    parser.add_argument("--server-address", default="localhost", help="The address of the server (default: localhost)")
    parser.add_argument("--port", type=int, default=8080, help="The port of the server (default: 8080)")
    
    args = parser.parse_args()
    
    download_file(args.server_address, args.port, args.filename, args.save_as)
