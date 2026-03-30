import socket

SERVER_HOST = 'localhost'
SERVER_PORT = 1111
def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((SERVER_HOST, SERVER_PORT))
        except Exception as e:
            print(f"Error connecting to server: {e}")
            return
        print("Connected to server.")
        try:
            data = s.recv(1024)
        except Exception as e:
            print(f"Error receiving data from server: {e}")
            return
        print(f"Received from server: {data.decode()}")
        while True:
            message = input("Enter message to print (the only message to server is 'hello' or 'exit' to quit): ")
            if message.lower() != 'hello' and message.lower() != 'exit':
                print(message)
                continue
            if message.lower() == 'exit':
                print("Exiting...")
                break
            if len(message) != 5:
                print("Invalid message length. Only 'hello' is accepted.")
                continue
            try:
                print(f"Sending to server: {message}")
                s.sendall(message.encode())
                data = s.recv(1024)
                print(f"Received from server: {data.decode()}")
            except Exception as e:
                print(f"Error sending/receiving data: {e}")
                break
                
if __name__ == "__main__":
    main()                