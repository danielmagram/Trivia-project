//#include "JsonResponsePacketSerializer.h"
//#include <vector>
//#include <iostream>
//#include <bitset>
//int main()
//{
//	ErrorResponse errorResponse{ "An error occurred" };
//	LoginResponse loginResponse{ 1 };
//	SignupResponse signupResponse{ 0 };
//	std::vector<unsigned char> errorPacket = JsonResponsePacketSerializer::serializeResponse(errorResponse);
//	std::vector<unsigned char> loginPacket = JsonResponsePacketSerializer::serializeResponse(loginResponse);
//	std::vector<unsigned char> signupPacket = JsonResponsePacketSerializer::serializeResponse(signupResponse);
//
//	std::cout << "Error Packet: ";
//	for (auto byte : errorPacket)
//	{
//		std::cout << std::bitset<8>(byte) << " ";
//	}
//	std::cout << "\n\nLogin Packet: ";
//	for (auto byte : loginPacket)
//	{
//		std::cout << std::bitset<8>(byte) << " ";
//	}
//	std::cout << "\n\nSignup Packet: ";
//	for (auto byte : signupPacket)
//	{
//		std::cout << std::bitset<8>(byte) << " ";
//	}
//
//	return 0;
//}