import socket
SERVER_IP = "127.0.0.1"
SERVER_PORT = 6969

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = (SERVER_IP,SERVER_PORT)

    sock.connect(server_address)

    server_msg = sock.recv(1024)
    server_msg = server_msg.decode()
    print(server_msg)

    if server_msg == "Hello":
        msg = "Hello!"
        sock.sendall(msg.encode())

    sock.close()

if __name__ == '__main__':
    main()