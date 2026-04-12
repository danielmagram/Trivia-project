import socket
import json
import struct

SERVER_HOST = 'localhost'
SERVER_PORT = 1111

LOGIN_CODE = 202
SIGNUP_CODE = 33


def send_request(sock, code, payload_dict):
    json_str = json.dumps(payload_dict)
    json_bytes = json_str.encode()

    size = struct.pack('!I', len(json_bytes))
    code_byte = struct.pack('B', code)

    message = code_byte + size + json_bytes
    sock.sendall(message)


def receive_response(sock):
    code = sock.recv(1)
    size_bytes = sock.recv(4)
    size = struct.unpack('!I', size_bytes)[0]

    data = b''
    while len(data) < size:
        chunk = sock.recv(size - len(data))
        if not chunk:
            raise Exception("Disconnected")
        data += chunk

    json_data = json.loads(data.decode())
    return json_data


def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((SERVER_HOST, SERVER_PORT))
        print("Connected to server.")

        while True:
            choice = input("login / signup / exit: ").lower()

            if choice == "exit":
                break

            username = input("username: ")
            password = input("password: ")

            if choice == "login":
                payload = {"username": username, "password": password}
                send_request(s, LOGIN_CODE, payload)

            elif choice == "signup":
                mail = input("email: ")
                payload = {"username": username, "password": password, "email": mail}
                send_request(s, SIGNUP_CODE, payload)

            else:
                print("Invalid option")
                continue

            response = receive_response(s)
            print("Server response:", response)


if __name__ == "__main__":
    main()
