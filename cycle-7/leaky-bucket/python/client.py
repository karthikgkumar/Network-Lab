import socket
import time

PORT = 8080
BUFFER_SIZE = 1024

def start_client():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', PORT))

    packet_number = 1
    try:
        while True:
            message = f"Packet {packet_number}"
            client_socket.send(message.encode('utf-8'))
            print(f"Sent: {message}")

            packet_number += 1

            # Adjust this sleep time to control the rate at which packets are sent
            time.sleep(0.5)  # Sleep for 500ms
    except KeyboardInterrupt:
        print("Client stopped by user")
    finally:
        client_socket.close()

if __name__ == "__main__":
    start_client()
