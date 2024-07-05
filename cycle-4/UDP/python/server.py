# server.py

import socket
import datetime

PORT = 8888
BUFFER_SIZE = 1024

def handle_client(server_socket, client_address):
    # Get current time
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    # Send time to client
    server_socket.sendto(current_time.encode(), client_address)
    print(f"Sent time to {client_address}")

def main():
    # Create UDP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Bind socket to address and port
    server_socket.bind(('', PORT))
    
    print(f"UDP Time Server listening on port {PORT}...")

    while True:
        # Receive request from client
        message, client_address = server_socket.recvfrom(BUFFER_SIZE)
        print(f"Received request from {client_address}")
        
        # Handle client request
        handle_client(server_socket, client_address)

if __name__ == "__main__":
    main()
