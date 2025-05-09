import socket
import os
import json

# function for uploading a file
def upload_file(filename):  
    # if the client tries to upload a file that doesn't exist, quit  
    if not os.path.exists(filename):
        print("File doesn't exist")
        return
    
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((pi_ip, pi_port))
    base_filename = os.path.basename(filename)

    # send message to server detailing op
    print("sending op message")
    client.send(json.dumps({"op":"up","filename":base_filename}).encode('utf-8'))

    # wait for confirmation server is ready to receive upload. this prevents op message and upload being send together and received as 1 message
    print("waiting for confirmation")
    _ = client.recv(transfer_buffer).decode('utf-8')

    # send file data
    with open(filename, 'rb') as file:
        while True:
            #print("sending upload data")
            data = file.read(transfer_buffer)
            if not data:
                break
            client.send(data)
    
    client.shutdown(socket.SHUT_WR)

    # get confirmation file was received by server
    _ = client.recv(transfer_buffer).decode('utf-8')     
    client.close()
    print("File uploaded")

# downloading file stuff. admin operation, so needs a password
def download_file(filename, password):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((pi_ip, pi_port))

    # send op information and wait for confirmation
    client.send(json.dumps({"op":"down","filename":filename,"password":password}).encode('utf-8'))
    response = client.recv(transfer_buffer).decode('utf-8')
    
    # if the response is anything but the "im ready" response, close connection and quit
    if response != "good":
        print (response)
        client.close()
        return
    
    # tell server client is good to start receiving data
    client.send("good".encode('utf-8'))
    
    # receive data. basically copy/pasted from server download function
    with open(filename, 'wb') as f:
        while True:
            data = client.recv(transfer_buffer)
            if not data:
                break
            f.write(data)

    client.shutdown(socket.SHUT_WR)
    
    client.close()
    # close connection to server
    print("File downloaded")

# function for listing files on server. also needs a password, cuz there's no reason for an uploader to check files on server
# pretty straightforward functionality, just gets a string from server and displays it
def list_files(password):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((pi_ip, pi_port))
    
    client.send(json.dumps({"op":"ls", "password":password}).encode('utf-8'))
    
    file_list = client.recv(transfer_buffer).decode('utf-8')
    print("Files available on server:")
    print(file_list)
    client.close()

# global varss
transfer_buffer = 4096

# ask user for input for destination ip and port. this is much better UX than having to change the program itself (if a bit annoying for testing)
pi_ip = input("What is the ip of the destination server? ") # pi network ip (CHECK AFTER EACH NETWORK CONNECTION AND CHANGE IF NECESSARY. localhost ip is 127.0.0.1

# this just needs to be the same as the port on the server. sometimes, certain ports are occupied with other stuff so it might fail, needing this to be changed (idrk why/when that happens though, it just seems to happen sometimes)
pi_port = int(input("What is the port of the destination server? ")) 

while True:
    print("")
    print("1. Upload files to server")
    print("2. List files on server")
    print("3. Download files from server")
    print("4. Quit")

    match input("Choice: "):
        case '1':
            filename = input("Enter the filename to upload: ")
            upload_file(filename)
        case '2':
            password = input("This is an admin operation! What is the password? ")
            print(password)
            list_files(password)
        case '3':
            password = input("This is an admin operation! What is the password? ")
            print(password)
            filename = input("Enter the filename to download: ")
            download_file(filename, password)
        case '4':
            print("Quitting...")
            break
        case _:
            "Invalid usage."