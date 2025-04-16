import socket
import json

SERVER_IP = "127.0.0.1"
SERVER_PORT = 6969


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = (SERVER_IP, SERVER_PORT)

    sock.connect(server_address)

    server_msg = sock.recv(1024)
    server_msg = server_msg.decode()
    print(server_msg)

    userInput = int(input("""Choose the options: \n1.Login \n2.SignUp \n"""))

    if userInput == 1:
        username = input('Enter your username: ')
        password = input('Enter your password: ')

        loginMsg = {
            "username": username,
            "password": password
        }

        loginMsgSerialized = json.dumps(loginMsg).encode('utf-8')
        loginMsgSize = len(loginMsgSerialized).to_bytes(4, 'big')  #i hate little endian

        opcode = 1
        opcodeByte = bytes([opcode])

        message = opcodeByte + loginMsgSize + loginMsgSerialized

        sock.sendall(message)
    elif userInput == 2:
        username = input('Enter your username: ')
        password = input('Enter your password: ')
        mail = input("Enter your email: ")

        signUpMsg = {
            "username": username,
            "password": password,
            "mail": mail
        }

        signUpMsgSerialized = json.dumps(signUpMsg).encode('utf-8')  #
        signUpMsgSize = len(signUpMsgSerialized).to_bytes(4, 'big')  #i hate little endian

        opcode = 1
        opcodeByte = bytes([opcode])

        message = opcodeByte + signUpMsgSize + signUpMsgSerialized

        sock.sendall(message)

    server_msg = sock.recv(1024)
    server_msg = server_msg.decode()
    print(server_msg)
    sock.close()


if __name__ == '__main__':
    main()
