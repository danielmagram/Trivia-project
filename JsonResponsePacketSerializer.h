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

class JsonResponsePacketSerializer
{
public:
    static std::vector<unsigned char> serializeErrorResponse(ErrorResponse response);
    static std::vector<unsigned char> serializeLoginResponse(LoginResponse response);
    static std::vector<unsigned char> serializeSignupResponse(SignupResponse response);

    static std::vector<unsigned char> serializeResponse(const LogoutResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetRoomsResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetPlayersInRoomResponse& response);
    static std::vector<unsigned char> serializeResponse(const JoinRoomResponse& response);
    static std::vector<unsigned char> serializeResponse(const CreateRoomResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetHighScoreResponse& response);
    static std::vector<unsigned char> serializeResponse(const GetPersonalStatsResponse& response);
};