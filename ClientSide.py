import socket
import json
import cbor2

SERVER_IP = "127.0.0.1"
SERVER_PORT = 6969


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = (SERVER_IP, SERVER_PORT)
    sock.connect(server_address)

    userInput = int(input("""Choose the options: \n1.Login \n2.SignUp \n"""))

    if userInput == 1:
        username = input('Enter your username: ')
        password = input('Enter your password: ')

        loginMsg = {
            "username": username,
            "password": password
        }

        loginMsgSerialized = cbor2.dumps(loginMsg)
        loginMsgSize = len(loginMsgSerialized).to_bytes(4, 'little')  #i hate little endian

        opcode = 0
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
            "email": mail
        }

        signUpMsgSerialized = cbor2.dumps(signUpMsg)
        signUpMsgSize = len(signUpMsgSerialized).to_bytes(4, 'little')  # i hate little endian

        opcode = 2
        opcodeByte = bytes([opcode])

        message = opcodeByte + signUpMsgSize + signUpMsgSerialized
        print(message)

        sock.sendall(message)

    server_msg = sock.recv(1024)
    code = server_msg[0]
    length = server_msg[1:5]
    length = int.from_bytes(length, "little")  # little endian byte order as always basically :C
    print("Code: ", code, "Length: ", length)
    if code == 0:
        print("successful login siir")
    elif code == 2:
        print("Successful sign up sirrrr")
    else:
        print("Login failed :C")

    sock.close()


if __name__ == '__main__':
    main()
