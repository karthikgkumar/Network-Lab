import socket
import os
from multiprocessing import Process

PORT = 8080
BUFFER_SIZE = 1024

def handle_client(client_socket):
    try:
        # Receive filename from client
        filename = client_socket.recv(BUFFER_SIZE).decode('utf-8')
        pid = os.getpid()

        # Check if file exists
        if os.path.exists(filename):
            with open(filename, 'rb') as file:
                # Send PID and file content
                client_socket.send(f"PID: {pid} - File content:\n".encode('utf-8'))
                while True:
                    data = file.read(BUFFER_SIZE)
                    if not data:
                        break
                    client_socket.send(data)
        else:
            # Send error message with PID
            error_msg = f"PID: {pid} - File not found: {filename}"
            client_socket.send(error_msg.encode('utf-8'))
    except Exception as e:
        print(f"Error handling client: {e}")
    finally:
        client_socket.close()

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(('', PORT))
    server_socket.listen(5)
    
    print(f"Server listening on port {PORT}")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"New connection from {addr}")
        
        # Create a new process to handle the client
        process = Process(target=handle_client, args=(client_socket,))
        process.daemon = True
        process.start()

if __name__ == "__main__":
    start_server()
