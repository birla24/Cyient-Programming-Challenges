import socket
import threading

def handle_client(client_socket, client_address):
    print(f"Connection established with {client_address}")
    while True:
        data = client_socket.recv(1024).decode('utf-8')
        if data == "ping":
            print(f"Received 'ping' from {client_address}")
            client_socket.send("pong".encode('utf-8'))

    client_socket.close()
    print(f"Connection closed with {client_address}")

def start_server(bind_address, bind_port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((bind_address, bind_port))
    server_socket.listen(5)
    print(f"Server listening on {bind_address}:{bind_port}")

    while True:
        client_socket, client_address = server_socket.accept()
        client_thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
        client_thread.start()

def main():
    bind_address = "127.0.0.1"
    bind_port = 7000

    start_server(bind_address, bind_port)

if __name__ == "__main__":
    main()