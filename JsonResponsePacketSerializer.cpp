#include "JsonResponsePacketSerializer.h"
#include "json.hpp" 
#include "Constants.h"
using json = nlohmann::json;


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


std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
    json j;
    j["message"] = response.message;

    return buildPacket(static_cast<unsigned char>(RequestCode::ERROR_CODE), j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(static_cast<unsigned char>(RequestCode::LOGIN), j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(static_cast<unsigned char>(RequestCode::SIGNUP), j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(static_cast<unsigned char>(RequestCode::LOGOUT), j);
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

    return buildPacket(static_cast<unsigned char>(RequestCode::GET_ROOMS), j);
}
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    json j;
    j["PlayersInRoom"] = response.players;
    return buildPacket(static_cast<unsigned char>(RequestCode::GET_PLAYERS), j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(static_cast<unsigned char>(RequestCode::JOIN_ROOM), j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
    json j;
    j["status"] = response.status;

    return buildPacket(static_cast<unsigned char>(RequestCode::CREATE_ROOM), j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
    json j;
    j["HighScores"] = response.statistics;
    j["Status"] = response.status;

    return buildPacket(static_cast<unsigned char>(RequestCode::GET_HIGHSCORE), j);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
    json j;
    j["UserStatistics"] = response.statistics;
	j["Status"] = response.status;

    return buildPacket(static_cast<unsigned char>(RequestCode::GET_PERSONAL_STATS), j);
}
