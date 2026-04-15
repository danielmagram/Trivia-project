import socket
import struct
import json
import time

SERVER_IP = 'localhost'
SERVER_PORT = 1111 

LOGIN_CODE = 202
SIGNUP_CODE = 33

def send_request(sock, code, data_dict):
    json_data = json.dumps(data_dict).encode('utf-8')
    size = len(json_data)
    header = struct.pack('>BI', code, size)
    sock.sendall(header + json_data)

def receive_response(sock):
    try:
        code_data = sock.recv(1)
        if not code_data:
            return "Server disconnected"
        code = struct.unpack('>B', code_data)[0]
        
        size_data = sock.recv(4)
        size = struct.unpack('>I', size_data)[0]
        
        json_data = b""
        while len(json_data) < size:
            json_data += sock.recv(size - len(json_data))
            
        return f"Code: {code}, Data: {json.loads(json_data.decode('utf-8'))}"
    except Exception as e:
        return f"Error reading response: {e}"

def run_test(test_name, code, payload):
    print(test_name)
    # Open a fresh connection for EVERY test
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect((SERVER_IP, SERVER_PORT))
        send_request(sock, code, payload)
        print("Response:", receive_response(sock), "\n")
    except Exception as e:
        print("Test failed:", e)
    finally:
        sock.close() # Close connection after the test
    time.sleep(0.1)

def main():
    print("Starting Server Tests...\n")

    # 1. Test Input Validation 
    run_test("--- Test 1: Signup with empty username ---", 
             SIGNUP_CODE, {"username": "", "password": "Password1!", "email": "test@test.com"})

    # 2. Test Normal Signup random username
    username = f"tester{int(time.time())}"  # Unique username based on timestamp
    run_test("--- Test 2: Normal Signup (Creating " + username + ") ---", 
             SIGNUP_CODE, {"username": username, "password": "Password1!", "email": "test@test.com"})

    # 3. Test duplicate Signup
    run_test("--- Test 3: Duplicate Signup (Trying to create " + username + " again) ---", 
             SIGNUP_CODE, {"username": username, "password": "Password1!", "email": "test@test.com"})

    # 4. Test Unregistered Login
    run_test("--- Test 4: Unregistered Login (Trying to login with 'fakeuser') ---", 
             LOGIN_CODE, {"username": "fakeuser", "password": "Password1!"})

    # 5. Test Normal Login
    run_test("--- Test 5: Normal Login (Logging in with " + username + ") ---", 
             LOGIN_CODE, {"username": username, "password": "Password1!"})

    # 6. user cant login if already logged in
    run_test("--- Test 6: Login while already logged in (Trying to login with " + username + " again) ---", 
             LOGIN_CODE, {"username": username, "password": "Password1!"})
    

if __name__ == "__main__":
    main()