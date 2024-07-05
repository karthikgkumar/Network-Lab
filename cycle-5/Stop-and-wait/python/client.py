# client.py

import socket
import struct
import time

SERVER_ADDRESS = 'localhost'
PORT = 8888
BUFFER_SIZE = 1024
TIMEOUT = 2.0

def main():
    # Create UDP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(TIMEOUT)

    seq_num = 0

    while True:
        message = input("Enter message (or 'quit' to exit): ")
        if message.lower() == 'quit':
            break

        packet = struct.pack(f'!I{BUFFER_SIZE}s', seq_num, message.encode())

        while True:
            # Send packet
            client_socket.sendto(packet, (SERVER_ADDRESS, PORT))
            print(f"Sent packet {seq_num}: {message}")

            try:
                # Wait for ACK
                ack, _ = client_socket.recvfrom(4)
                ack_num = struct.unpack('!I', ack)[0]

                if ack_num == seq_num:
                    print(f"Received ACK {ack_num}")
                    seq_num = 1 - seq_num  # Toggle between 0 and 1
                    break
                else:
                    print(f"Received incorrect ACK. Resending packet {seq_num}")
            except socket.timeout:
                print(f"Timeout. Resending packet {seq_num}")

    client_socket.close()

if __name__ == "__main__":
    main()
