import socket
import sys

PORT = 8080
BUFFER_SIZE = 1024

def request_file(filename):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        client_socket.connect(('localhost', PORT))
        
        # Send filename to server
        client_socket.send(filename.encode('utf-8'))
        
        # Receive and print server response
        while True:
            data = client_socket.recv(BUFFER_SIZE)
            if not data:
                break
            sys.stdout.buffer.write(data)
            sys.stdout.flush()
    
    except Exception as e:
        print(f"Error: {e}")
    finally:
        client_socket.close()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <filename>")
        sys.exit(1)
    
    filename = sys.argv[1]
    request_file(filename)

"""This Python implementation provides the same functionality as the C version:

The server handles multiple clients concurrently using the multiprocessing module.
The server sends its PID along with the file content or error message.
If the requested file doesn't exist, the server sends an appropriate error message.
The client can request a file by providing its name as a command-line argument.

Key differences and notes:

Python's multiprocessing module is used instead of fork() for better cross-platform compatibility.
Error handling is implemented using try-except blocks.
The server uses os.path.exists() to check if the file exists.
The client uses sys.stdout.buffer.write() to handle both text and binary data.

Make sure to create some test files in the same directory as the server for testing purposes. The client will display the server's PID and either the file content or an error message, depending on whether the file exists or not.
This Python implementation is more concise and may be easier to read and maintain compared to the C version, while providing the same functionality."""
