import socket
import psutil

def get_network_interfaces():
    interfaces = psutil.net_if_addrs()
    
    for interface_name, interface_addresses in interfaces.items():
        print(f"Interface: {interface_name}")
        for addr in interface_addresses:
            if addr.family == socket.AF_INET:  # IPv4
                print(f"\tIPv4 Address: {addr.address}")
            elif addr.family == socket.AF_INET6:  # IPv6
                print(f"\tIPv6 Address: {addr.address}")
            elif addr.family == psutil.AF_LINK:  # MAC address
                print(f"\tMAC Address: {addr.address}")
        print()

if __name__ == "__main__":
    get_network_interfaces()

"""
This Python script does the following:

It imports the socket module for network-related constants and the psutil (Python System and Process Utilities) library for getting system information.
The get_network_interfaces() function is defined to retrieve and display network interface information.
Inside the function, psutil.net_if_addrs() is called to get a dictionary of network interfaces and their addresses.
It then iterates through each interface and its associated addresses.
For each address, it checks the address family:

If it's IPv4 (AF_INET), it prints the IPv4 address.
If it's IPv6 (AF_INET6), it prints the IPv6 address.
If it's a MAC address (psutil.AF_LINK), it prints the MAC address.


Finally, in the if __name__ == "__main__": block, it calls the function to display the information when the script is run directly."""
