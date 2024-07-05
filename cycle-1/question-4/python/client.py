import socket

def get_remote_mac(host, port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect((host, port))
        mac_address = client_socket.recv(1024).decode()
        return mac_address
    except Exception as e:
        print(f"Error: {e}")
        return None
    finally:
        client_socket.close()

def main():
    host = input("Enter the IP address of the server: ")
    port = 12345  # Same port as server

    mac_address = get_remote_mac(host, port)
    if mac_address:
        print(f"The MAC address of {host} is: {mac_address}")
    else:
        print(f"Could not retrieve the MAC address for {host}")

if __name__ == "__main__":
    main()

"""Output
Enter the IP address of the server: 127.0.0.1
The MAC address of 127.0.0.1 is: cc:6b:1e:30:74:b4"""
