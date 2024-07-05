import socket
import uuid

def get_mac_address():
    return ':'.join(['{:02x}'.format((uuid.getnode() >> elements) & 0xff) 
                     for elements in range(0,48,8)][::-1])

def start_server():
    host = '0.0.0.0'  # Listen on all available interfaces
    port = 12345

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((host, port))
    server_socket.listen(1)

    print(f"Server listening on {host}:{port}")

    while True:
        client_socket, address = server_socket.accept()
        print(f"Connection from {address}")
        
        mac_address = get_mac_address()
        client_socket.send(mac_address.encode())
        
        client_socket.close()

if __name__ == "__main__":
    start_server()
