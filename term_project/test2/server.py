import os
import socket
import json

HOST = '0.0.0.0'
PORT = 8000

clients = {}

def connect_client(sock, addr, name):
    if name in clients:
        return

    clients[name] = {
        'socket': sock,
        'address': addr
    } 

    sock.send("Connected".encode('utf-8'))

    print("Client registered")

def disconnect_client(sock, inputs):
    client = None
    for key, value in clients.items():
        if value['socket'] == sock:
            client = key
            break

    if client:
        del clients[client]
        print("Client disconnected")

    if sock in inputs:
        inputs.remove(sock)
    sock.close() 



def main():
    server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_sock.bind((HOST, PORT))
    server_sock.setblocking(0)
    server_sock.listen(5)

    print("Server started...")

    inputs = [server_sock]

    try:
        while inputs:
            

    
