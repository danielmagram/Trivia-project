#include "JsonResponsePacketSerializer.h"
#include "json.hpp" 
using json = nlohmann::json;

constexpr unsigned char ERROR_CODE = 40;
constexpr unsigned char LOGIN_CODE = 202;
constexpr unsigned char SIGNUP_CODE = 33;
constexpr unsigned char GET_ROOMS_CODE = 101;
constexpr unsigned char GET_HIGHSCORE_CODE = 140;
constexpr unsigned char GET_PERSONAL_STATS_CODE = 150;
constexpr unsigned char GET_PLAYERS_CODE = 102;
//HELPER FUNCTION
std::vector<unsigned char> buildPacket(unsigned char code, const json& j)
{
    std::string jsonString = j.dump();
    std::vector<unsigned char> packet;

    packet.push_back(code);

    unsigned int size = jsonString.length();
    packet.push_back((size >> 24) & 0xFF);
    packet.push_back((size >> 16) & 0xFF);
    packet.push_back((size >> 8) & 0xFF);
    packet.push_back((size >> 0) & 0xFF);

    packet.insert(packet.end(), jsonString.begin(), jsonString.end());

    return packet;
}


std::vector<unsigned char> JsonResponsePacketSerializer::serializeErrorResponse(ErrorResponse response)
{
    json j;
    j["message"] = response.message;

    return buildPacket(ERROR_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeLoginResponse(LoginResponse response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(LOGIN_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeSignupResponse(SignupResponse response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(SIGNUP_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(SIGNUP_CODE, j);
}
// fix for rooms data
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
    json j;
    std::string roomsString = "";

    for (size_t i = 0; i < response.rooms.size(); ++i) {
        roomsString += response.rooms[i].name;
        if (i < response.rooms.size() - 1) {
            roomsString += ", ";
        }
    }

    j["Rooms"] = roomsString;

    return buildPacket(GET_ROOMS_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    json j;
    std::string playersString = "";

    for (size_t i = 0; i < response.players.size(); ++i) {
        playersString += response.players[i];
        if (i < response.players.size() - 1) {
            playersString += ", ";
        }
    }

    j["PlayersInRoom"] = playersString;

    return buildPacket(GET_PLAYERS_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(SIGNUP_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(SIGNUP_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
    json j;
    j["HighScores"] = response.statistics;

    return buildPacket(GET_HIGHSCORE_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
    json j;
    j["UserStatistics"] = response.statistics;

    return buildPacket(GET_PERSONAL_STATS_CODE, j);
}
