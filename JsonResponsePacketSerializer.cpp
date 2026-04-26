#include "JsonResponsePacketSerializer.h"
#include "json.hpp" 
using json = nlohmann::json;

constexpr unsigned char ERROR_CODE = 40;
constexpr unsigned char LOGIN_CODE = 202;
constexpr unsigned char SIGNUP_CODE = 33;
constexpr unsigned char LOGOUT_CODE = 203;
constexpr unsigned char JOIN_ROOM_CODE = 100;
constexpr unsigned char CREATE_ROOM_CODE = 100;
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

    return buildPacket(LOGOUT_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
    json j;
    j["status"] = response.status; 

    json roomsArray = json::array(); 

    for (const auto& room : response.rooms) {
        json roomJson;

        roomJson["id"] = room.id;
        roomJson["name"] = room.name;
        roomJson["maxPlayers"] = room.maxPlayers;
        roomJson["numOfQuestionsInGame"] = room.numOfQuestionsInGame;
        roomJson["timePerQuestion"] = room.timePerQuestion;
        roomJson["isActive"] = room.status;

        roomsArray.push_back(roomJson);
    }

    j["Rooms"] = roomsArray;

    return buildPacket(GET_ROOMS_CODE, j);
}
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    json j;
    j["PlayersInRoom"] = response.players;
    return buildPacket(GET_PLAYERS_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(JOIN_ROOM_CODE, j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(CREATE_ROOM_CODE, j);
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
