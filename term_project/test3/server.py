import socket
import os
import threading
import json
import api_test
from pathlib import Path

# global variabled needed in multiple functions
transfer_buffer = 4096
download_folder_name = 'GOOD'
pi_ip = '127.0.0.1' # Localhost IP
#pi_ip = '127.0.0.1' # CHANGE THIS BASED ON PI NETWORK IP (use ifconfig in py terminal to find pi ip)
pi_port = 8000 # the port the pi is using on the network. should generally be fine with 8000, but if client connection is refused try a different port
password = "password" # obviously this isn't secure at all but just for a demo

# general hub to process a connection fron a client
def process_conn(conn):

    # receive the initial message and extract the op field to decide what to do with it
    # this structure allows the client to enter any command with a generic message (just change the op value in the json)
    msg = json.loads(conn.recv(transfer_buffer).decode('utf-8'))
    op = msg.get("op")
    print(op)

    # do action based on op
    if op == 'up':
        filename = msg.get("filename")
        up(conn, filename)    
    # since we only want an admin/trusted user to be able to download files from the server, make them enter a password
    elif op == 'down' and msg.get("password") == password:
        filename = msg.get("filename")
        down(conn, filename)   
    # also need to enter a password to list files since there's no reason an uploader would need to check the file list, they get a confirmation that their file was uploaded successfully in the upload command
    elif op == 'ls' and msg.get("password") == password:
        list(conn)
    else:
        # close the connection if op isn't valid
        conn.close()
        print("Disconnected")

# function to upload a file to the server
def up(conn, filename):   
    # let the client know it's okay to start sending data
    # this prevents the client sending data sequentially and the sever perceiving it as one message      
    print("sending confirmation")  
    conn.send("good".encode('utf-8'))
    file_path = os.path.join('TEMP', filename)
    
    with open(file_path, 'wb') as f:
        while True:
            # receive next data packet from client
            #print("waiting for upload data")
            data = conn.recv(transfer_buffer)
            if not data:
                break
            # if packet ends in EOF bytecode, this is the last packet so write it (without EOF ) and end transfer
            # could also be done by sleeping for a bit after sending all data then sending a "done" message or smth but this seems a bit more robust (using time.sleep() for anything networking seems very bad practice)
            if data.endswith(b'EOF'): 
                #print(data[:-3])
                f.write(data[:-3]) # write the data (WITHOUT THE ADDED EOF SO YOU AREN'T MODIFYING THE FILE CONTENT IDIOT)
                break
            #print(data)
            f.write(data)
    
    print("File received")
    conn.send("File received".encode('utf-8')) # tell client that the file was received and close
    conn.close() # close before sending file to api tester cuz that function takes a WHILE and there's no reason for the client to be sitting there connected the whole time
    print("Disconnected")
    #api_test.main(filename) # send the file through to the api malware tester

# function for downloading files from the server
def down(conn, filename):
    file_path = os.path.join(download_folder_name, filename)
            
    # make sure the file is on the server
    if os.path.exists(file_path):
        _ = conn.send("good".encode('utf-8')) # let client know the file exists
        conn.recv(transfer_buffer) # wait for the client to be ready to receive file
        
        # send file to client
        with open(file_path, 'rb') as f:
            while True:
                data = f.read(transfer_buffer)
                if not data:
                    break
                conn.send(data)
        
        conn.send(b'EOF') # send EOF as bytecode to let the client know that the file is over
        print("File sent")
        os.remove(file_path) # remove the file from the server
        print("File deleted from server")
    else:
        # if the file doesn't exist, tell the client that they are stupid
        conn.send("file not found".encode('utf-8'))

# function to list the files on the server. this allows the admin user to see what files are available to download (since a filename is needed)
def list(conn):
    files = os.listdir(download_folder_name)
    print(files)
    file_list = '\n'.join(files) if files else "No files available"
    conn.send(file_list.encode('utf-8'))
    print("File list sent")


# setting up server stuff
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((pi_ip, pi_port))
server.listen(5)
server.settimeout(1.0) # set a timeout to recheck for changes. this + the KeyboardInterrupt catch allows us to close the server with CTRL+C
print("Server started...")

# if the necessary folders don't exist, make them
Path('TEMP').mkdir(exist_ok=True)
Path('BAD').mkdir(exist_ok=True)
Path('CHECK').mkdir(exist_ok=True)
Path('GOOD').mkdir(exist_ok=True)

# main server loop
try:
    while True:
        try:
            client_sock, _ = server.accept() # accept a connection from a client

            # create a new thread for the client. this allows multiple clients to not get stuck blocking if there is another client 
            # trying to connect and the same time (and just removes a lot of server errors/weirdness)
            # threads are daemons so that they automatically shut down when the main server program shuts down.
            t = threading.Thread(target=process_conn, args=(client_sock,), daemon=True) 
            t.start()
        # if server times out, don't do anything (since the timeout is just to check for keyboard interrupts)
        except socket.timeout:
            continue
# if there is a keyboard interrupt, break out of loop to kill the server
except KeyboardInterrupt:
    print("Server killed")
except Exception as e:
    print("Error: ", e)
server.close() # close the server as last action
