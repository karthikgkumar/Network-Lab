# server.py

import socket

def start_server():
    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Define the host and port
    host = 'localhost'
    port = 12345

    # Bind the socket to a specific address and port
    server_socket.bind((host, port))

    # Listen for incoming connections
    server_socket.listen(1)

    print(f"Server listening on {host}:{port}")

    # Wait for a client connection
    client_socket, client_address = server_socket.accept()
    print(f"Connected to client: {client_address}")

    try:
        while True:
            # Receive data from the client
            data = client_socket.recv(1024).decode()
            if not data:
                break
            print(f"Received: {data}")

            # Send an acknowledgment back to the client
            response = f"Server received: {data}"
            client_socket.send(response.encode())

    except Exception as e:
        print(f"An error occurred: {e}")

    finally:
        # Close the connections
        client_socket.close()
        server_socket.close()
        print("Server shut down")

if __name__ == "__main__":
    start_server()
