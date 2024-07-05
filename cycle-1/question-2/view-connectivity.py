import subprocess
import platform
import socket

def ping(host):
    """
    Returns True if host (str) responds to a ping request.
    Remember that a host may not respond to a ping (ICMP) request even if the host name is valid.
    """
    # Option for the number of packets as a function of
    param = '-n' if platform.system().lower()=='windows' else '-c'

    # Building the command. Ex: "ping -c 1 google.com"
    command = ['ping', param, '1', host]

    return subprocess.call(command) == 0

def get_ip_address(host):
    try:
        return socket.gethostbyname(host)
    except socket.gaierror:
        return None

def test_connectivity(hosts):
    for host in hosts:
        ip = get_ip_address(host)
        if ip:
            print(f"\nTesting connectivity to {host} ({ip})")
            if ping(host):
                print(f"Successfully pinged {host}")
            else:
                print(f"Failed to ping {host}")
        else:
            print(f"\nCould not resolve hostname for {host}")

if __name__ == "__main__":
    hosts = ["8.8.8.8", "1.1.1.1", "208.67.222.222"]  # Google DNS, Cloudflare DNS, OpenDNS
    test_connectivity(hosts)
