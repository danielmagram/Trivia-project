//#include <iostream>
//#include <vector>
//#include <string>
//#include "JsonResponsePacketSerializer.h"
//#include "JsonRequestPacketDeserializer.h"
//
//void printBuffer(const std::vector<unsigned char>& buffer)
//{
//    std::cout << "[ ";
//    for (unsigned char byte : buffer)
//    {
//        if (byte >= 32 && byte <= 126)
//            std::cout << "'" << byte << "' ";
//        else
//            std::cout << (int)byte << " ";
//    }
//    std::cout << "]\n";
//}
//
//int main()
//{
//    std::cout << "=== TESTING SERIALIZATION (Server to Client) ===\n";
//
//    LoginResponse loginRes;
//    loginRes.status = 1;
//
//    std::vector<unsigned char> outBuffer = JsonResponsePacketSerializer::serializeLoginResponse(loginRes);
//
//    std::cout << "Serialized LoginResponse (Status: 1):\n";
//    printBuffer(outBuffer);
//    std::cout << "Notice the 1-byte code (e.g., 202), followed by 4 bytes of size, followed by the JSON string!\n\n";
//
//
//    std::cout << "=== TESTING DESERIALIZATION (Client to Server) ===\n";
//
//
//    std::string fakeClientJson = R"({"username": "myUser123", "password": "myPassword!"})";
//    std::vector<unsigned char> inBuffer(fakeClientJson.begin(), fakeClientJson.end());
//
//    LoginRequest loginReq = JsonRequestPacketDeserializer::deserializeLoginRequest(inBuffer);
//
//    std::cout << "Deserialized LoginRequest:\n";
//    std::cout << "Username extracted: " << loginReq.username << "\n";
//    std::cout << "Password extracted: " << loginReq.password << "\n";
//
//    return 0;
//}