#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "Room.h"

struct ErrorResponse
{
    std::string message;
};

struct LoginResponse
{
    unsigned int status;
};

struct SignupResponse
{
    unsigned int status;
};

struct LogoutResponse {
    unsigned int status;
};

struct GetRoomsResponse {
    unsigned int status;
    std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse {
    std::vector<std::string> players;
};

struct JoinRoomResponse {
    unsigned int status;
};

struct CreateRoomResponse {
    unsigned int status;
};

struct GetHighScoreResponse {
    unsigned int status;
    std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse {
    unsigned int status;
    std::vector<std::string> statistics;
};

struct CloseRoomResponse {
    unsigned int status;
};

struct StartGameResponse {
    unsigned int status;
};

struct GetRoomStateResponse
{
    unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	float answerTimeout;
};

struct LeaveRoomResponse
{
    unsigned int status;
};

class JsonResponsePacketSerializer
{
public:
    static std::vector<unsigned char> serializeResponse(ErrorResponse response);
    static std::vector<unsigned char> serializeResponse(LoginResponse response);
    static std::vector<unsigned char> serializeResponse(SignupResponse response);

    static std::vector<unsigned char> serializeResponse(const LogoutResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetRoomsResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetPlayersInRoomResponse& response);
    static std::vector<unsigned char> serializeResponse(const JoinRoomResponse& response);
    static std::vector<unsigned char> serializeResponse(const CreateRoomResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetHighScoreResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetPersonalStatsResponse& response);
	static std::vector<unsigned char> serializeResponse(const CloseRoomResponse& response);
	static std::vector<unsigned char> serializeResponse(const StartGameResponse& response);
	static std::vector<unsigned char> serializeResponse(const GetRoomStateResponse& response);
	static std::vector<unsigned char> serializeResponse(const LeaveRoomResponse& response);

};