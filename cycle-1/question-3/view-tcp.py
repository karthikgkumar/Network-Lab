import time
import psutil

def print_connections():
    print("Active TCP connections:")
    print("Local Address\t\tRemote Address\t\tStatus")
    
    for conn in psutil.net_connections(kind='tcp'):
        if conn.laddr and conn.raddr:
            local = f"{conn.laddr.ip}:{conn.laddr.port}"
            remote = f"{conn.raddr.ip}:{conn.raddr.port}"
            status = conn.status
            print(f"{local:<23}{remote:<23}{status}")

def main():
    print("Please visit a website now...")
    time.sleep(10)  # Wait for 10 seconds
    print_connections()

if __name__ == "__main__":
    main()
