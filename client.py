import socket
import time
import sys

def start_client(server_address, server_port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect((server_address, server_port))
        print(f"Connected to {server_address}:{server_port}")

        while True:
            client_socket.send("ping".encode('utf-8'))
            print("Sent 'ping'")
            response = client_socket.recv(1024).decode('utf-8')
            print(f"Received '{response}'")
            time.sleep(1)

    except Exception as e:
        print(f"Error: {e}")
    finally:
        client_socket.close()
        print("Connection closed")

def main():
    if len(sys.argv) != 3:
        print("Usage: python client.py <server_address> <server_port>")
        return

    server_address = sys.argv[1]
    server_port = int(sys.argv[2])

    start_client(server_address, server_port)

if __name__ == "__main__":
    main()