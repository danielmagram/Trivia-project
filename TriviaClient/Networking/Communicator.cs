using System.Net.Sockets;
using System.Text;

namespace TriviaClient.Networking
{
    public class Communicator
    {
        private static Communicator _instance;
        public static Communicator Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new Communicator();
                return _instance;
            }
        }

        private TcpClient _client;
        private NetworkStream _stream;

        private Communicator() { }


        public void Connect(string ip, int port)
        {
            try
            {
                _client = new TcpClient();
                _client.Connect(ip, port);
                _stream = _client.GetStream();
            }
            catch (Exception ex)
            {
                throw new Exception($"Failed to connect to server: {ex.Message}");
            }
        }

        public void SendRequest(byte code, string jsonPayload)
        {
            if (_stream == null || !_stream.CanWrite)
                throw new Exception("Not connected to the server.");

            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonPayload);

            // 1. Prepare the 5-byte header array
            byte[] header = new byte[5];

            // 2. First byte is the code
            header[0] = code;

            // 3. Get the length bytes and convert to Big-Endian 
            byte[] lengthBytes = BitConverter.GetBytes(jsonBytes.Length);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(lengthBytes);
            }

            // 4. Copy the reversed length bytes into the header
            Array.Copy(lengthBytes, 0, header, 1, 4);

            // 5. Send Header (5 bytes)
            _stream.Write(header, 0, 5);

            // 6. Send the JSON payload (if there is one)
            if (jsonBytes.Length > 0)
            {
                _stream.Write(jsonBytes, 0, jsonBytes.Length);
            }
        }


        public ResponseInfo ReceiveResponse()
        {
            if (_stream == null || !_stream.CanRead)
                throw new Exception("Not connected to the server.");

            // 1. Read the 5-byte header robustly
            byte[] header = new byte[5];
            int totalBytesRead = 0;
            while (totalBytesRead < 5)
            {
                int bytesRead = _stream.Read(header, totalBytesRead, 5 - totalBytesRead);
                if (bytesRead == 0) throw new Exception("Server disconnected unexpectedly.");
                totalBytesRead += bytesRead;
            }

            // 2. Extract the code
            byte code = header[0];

            // 3. Extract length and convert back to Little-Endian for C#
            byte[] lengthBytes = new byte[4];
            Array.Copy(header, 1, lengthBytes, 0, 4);
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(lengthBytes);
            }
            int dataLength = BitConverter.ToInt32(lengthBytes, 0);

            // 4. Read the JSON payload robustly
            string jsonResponse = "";
            if (dataLength > 0)
            {
                byte[] dataBytes = new byte[dataLength];
                totalBytesRead = 0;
                while (totalBytesRead < dataLength)
                {
                    int bytesRead = _stream.Read(dataBytes, totalBytesRead, dataLength - totalBytesRead);
                    if (bytesRead == 0) throw new Exception("Server disconnected during data payload.");
                    totalBytesRead += bytesRead;
                }
                jsonResponse = Encoding.UTF8.GetString(dataBytes);
            }

            return new ResponseInfo { Code = code, JsonPayload = jsonResponse };
        }

        public void Disconnect()
        {
            _stream?.Close();
            _client?.Close();
        }
    }

    public struct ResponseInfo
    {
        public byte Code;
        public string JsonPayload;
    }
}