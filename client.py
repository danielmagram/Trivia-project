import socket
import json
import struct

SERVER_HOST = 'localhost'
SERVER_PORT = 1111

LOGIN_CODE = 202
SIGNUP_CODE = 33

## need to print full response from server, not just status code


def send_request(sock, code, payload_dict):
    json_str = json.dumps(payload_dict)
    json_bytes = json_str.encode()

    size = struct.pack('!I', len(json_bytes))
    code_byte = struct.pack('B', code)

    message = code_byte + size + json_bytes
    sock.sendall(message)


def receive_response(sock):
    code_byte = sock.recv(1)
    if not code_byte:
        return None

    code = struct.unpack('B', code_byte)[0]

    size_bytes = sock.recv(4)
    if not size_bytes:
        return None

    size = struct.unpack('!I', size_bytes)[0]

    json_bytes = b''
    while len(json_bytes) < size:
        chunk = sock.recv(size - len(json_bytes))
        if not chunk:
            return None
        json_bytes += chunk

    json_str = json_bytes.decode()
    response_dict = json.loads(json_str)

    return {"code": code, "data": response_dict}

def main():
## tries the 4 cases mentioned above, and prints the server response for each case
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:

        sock.connect((SERVER_HOST, SERVER_PORT))

        send_request(sock, SIGNUP_CODE, {"username": "", "password": "pass2"})
        response = receive_response(sock)
        print("Signup with empty username:", response)    

        send_request(sock, SIGNUP_CODE, {"username": "user1", "password": "pass1"})
        response = receive_response(sock)
        print("Signup with existing username:", response)

        send_request(sock, LOGIN_CODE, {"username": "user24234", "password": "pass1"})
        response = receive_response(sock)
        print("Login without registration:", response)

        send_request(sock, LOGIN_CODE, {"username": "user1", "password": "pass1"})
        response = receive_response(sock)
        print("Login with correct credentials:", response)

        send_request(sock, LOGIN_CODE, {"username": "user1", "password": "pass1"})
        response = receive_response(sock)
        print("Login while already logged in:", response)



if __name__ == "__main__":
    main()
