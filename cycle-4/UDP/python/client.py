# client.py

import socket

SERVER_ADDRESS = 'localhost'
PORT = 8888
BUFFER_SIZE = 1024

def main():
    # Create UDP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Send time request to server
    message = "Request time"
    client_socket.sendto(message.encode(), (SERVER_ADDRESS, PORT))
    print("Time request sent to server.")
    
    # Receive time from server
    data, server_address = client_socket.recvfrom(BUFFER_SIZE)
    server_time = data.decode()
    
    print(f"Server's time: {server_time}")
    
    # Close the socket
    client_socket.close()

if __name__ == "__main__":
    main()
