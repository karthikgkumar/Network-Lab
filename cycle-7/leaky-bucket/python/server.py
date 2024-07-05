import socket
import time
import threading

PORT = 8080
BUFFER_SIZE = 1024
BUCKET_SIZE = 10
OUTPUT_RATE = 1  # packets per second

class LeakyBucket:
    def __init__(self):
        self.bucket = 0
        self.lock = threading.Lock()
        self.last_process_time = time.time()

    def add_to_bucket(self):
        with self.lock:
            current_time = time.time()
            time_diff = current_time - self.last_process_time

            # Leak the bucket
            leaked = int(time_diff * OUTPUT_RATE)
            self.bucket = max(0, self.bucket - leaked)

            # Try to add the new packet to the bucket
            if self.bucket < BUCKET_SIZE:
                self.bucket += 1
                self.last_process_time = current_time
                return True
            else:
                return False

def process_packet(packet):
    print(f"Processing packet: {packet}")
    # Simulate packet processing
    time.sleep(0.1)  # Sleep for 100ms

def handle_client(conn, addr, leaky_bucket):
    print(f"New connection from {addr}")
    while True:
        try:
            data = conn.recv(BUFFER_SIZE)
            if not data:
                break

            if leaky_bucket.add_to_bucket():
                process_packet(data.decode('utf-8'))
                print(f"Bucket size: {leaky_bucket.bucket}")
            else:
                print("Packet dropped. Bucket is full.")

        except Exception as e:
            print(f"Error handling client: {e}")
            break

    conn.close()

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind(('', PORT))
    server_socket.listen(5)

    print(f"Server listening on port {PORT}")

    leaky_bucket = LeakyBucket()

    while True:
        conn, addr = server_socket.accept()
        client_thread = threading.Thread(target=handle_client, args=(conn, addr, leaky_bucket))
        client_thread.start()

if __name__ == "__main__":
    start_server()
