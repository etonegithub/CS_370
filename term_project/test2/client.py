import os
import socket
import json
import sys

HOST_IP = '' # input acutal pi ip
HOST_PORT = 8000
BUFF = 1024

def connect(name):
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_sock.connect((HOST_IP, HOST_PORT))
        client_sock.send(name.encode('utf-8'))

        resp = client_sock.recv(BUFF).decode('utf-8')
        if not resp:
            client_sock.close()

        return client_sock
    except Exception:
        if client_sock:
            client_sock.close()
        return None
    
def send_file(client_sock, receiver, file):

    try:
        command = f"send:{receiver}:{os.path.basename(file)}"
        client_sock.send(command.encode('utf-8'))

        resp = client_sock.recv(BUFF).decode('utf-8')

        if not resp:
            print("Server err")
            return
        
        with open(file, 'rb') as f:
            while True:
                data = file.read(BUFF)
                if not data:
                    break
                client_sock.send(data)

        client_sock.send(b'EOF')

        resp = client_sock.recv(BUFF).decode('utf-8')
        print("Response: ", resp)
    except Exception:
        print("Error sending file")

if __name__ == "__main__":
    name = sys.argv[1]
    client_sock = connect(name)

    if client_sock:
        print("Connected to server")