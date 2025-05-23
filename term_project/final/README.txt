
USAGE:      server.py and api_test.py run on the Raspberry Pi, client.py runs on the desktops connecting to the pi. desktops must have the pi's
            local ip address and port to connect, as well as a password for some admin operations. server.py can be killed with CTRL+C, client.py
            can be killed by entering option 4 in the menu


SERVER COMMAND STRUCTURE:

    Start server:               python server.py
                                input server local IP, port, and admin password

    Finding Pi local IP:        cmd >ifconfig 

    Port:                       any port theoretically works, but sometimes certain ports can be occupied for other tasks. if this is the case,
                                just start up the server on another port


CLIENT COMMAND STRUCTURES:

    Start client:               python client.py
                                terminal menu directs user from there. the server IP and port must be gotten from whoever started the server
