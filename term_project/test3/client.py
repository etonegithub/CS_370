import socket
import os
import sys
import json

# global varss
#pi_ip = '129.82.30.127' # pi network ip (CHECK AFTER EACH NETWORK CONNECTION AND CHANGE IF NECESSARY)
pi_ip = '127.0.0.1' # Localhost IP
pi_port = 8000 # this just needs to be the same as the port on the server. sometimes, certain ports are occupied with other stuff so it might fail, needing this to be changed (idrk why/when that happens though, it just seems to happen sometimes)
transfer_buffer = 4096

# function for uploading a file
def upload_file(filename):  
    # if the client tries to upload a file that doesn't exist, quit  
    if not os.path.exists(filename):
        print("File doesn't exist")
        return
    
    try:
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
        
        client.send(b'EOF') # send EOF to signal end of file data
    
        # get confirmation file was received by server
        res = client.recv(transfer_buffer).decode('utf-8')
        print(res)     
    except Exception as e:
        print(e)
    client.close()

# downloading file stuff. admin operation, so needs a password
def download_file(filename, password):
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
            if data.endswith(b'EOF'):
                #print(data[:-3])
                f.write(data[:-3])
                break
            f.write(data)
    
    # close connection to server
    print("File downloaded")
    client.close()

# function for listing files on server. also needs a password, cuz there's no reason for an uploader to check files on server
# pretty straightforward functionality, just gets a string from server and displays it
def list_files(password):
    client.connect((pi_ip, pi_port))
    
    client.send(json.dumps({"op":"ls", "password":password}).encode('utf-8'))
    
    file_list = client.recv(transfer_buffer).decode('utf-8')
    print("Files available on server:")
    print(file_list)
    client.close()

# when client starts, automatically connect to server
global client 
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# check command args for client to confirm there are enough args for each command and split commands based on op
if len(sys.argv) < 3:
    print("Invalid usage. Refer to README.md")
elif sys.argv[1] == 'ls':
    list_files(sys.argv[2]) # param is password
elif sys.argv[1] == 'upload' and len(sys.argv) == 3:
    upload_file(sys.argv[2]) # param is filename
elif sys.argv[1] == 'download' and len(sys.argv) == 4:
    download_file(sys.argv[2], sys.argv[3]) # params are filename and password
else:
    print("Invalid usage. Refer to README.md")