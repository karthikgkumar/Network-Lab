# client.py

import socket

def start_client():
    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Define the server's host and port
    host = 'localhost'
    port = 12345

    try:
        # Connect to the server
        client_socket.connect((host, port))
        print(f"Connected to server at {host}:{port}")

        while True:
            # Get user input
            message = input("Enter a message (or 'quit' to exit): ")
            
            if message.lower() == 'quit':
                break

            # Send data to the server
            client_socket.send(message.encode())

            # Receive the response from the server
            response = client_socket.recv(1024).decode()
            print(f"Server response: {response}")

    except Exception as e:
        print(f"An error occurred: {e}")

    finally:
        # Close the connection
        client_socket.close()
        print("Disconnected from server")

if __name__ == "__main__":
    start_client()
