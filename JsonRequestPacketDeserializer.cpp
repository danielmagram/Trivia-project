#include "JsonRequestPacketDeserializer.h"
#include "json.hpp"

using json = nlohmann::json;

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& buffer)
{
    LoginRequest req;

    json j = json::parse(buffer.begin(), buffer.end());

    req.username = j["username"];
    req.password = j["password"];

    return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignUpRequest(const std::vector<unsigned char>& buffer)
{
    SignupRequest req;

    json j = json::parse(buffer.begin(), buffer.end());

    req.username = j["username"];
    req.password = j["password"];
    req.email = j["email"];

    return req;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const std::vector<unsigned char>& buffer)
{
    GetPlayersInRoomRequest req;
    json j = json::parse(buffer.begin(), buffer.end());

    req.roomId = j["roomId"];

    return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& buffer)
{
    JoinRoomRequest req;
    json j = json::parse(buffer.begin(), buffer.end());

    req.roomId = j["roomId"];

    return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& buffer)
{
    CreateRoomRequest req;
    json j = json::parse(buffer.begin(), buffer.end());

    req.roomName = j["roomName"];
    req.maxUsers = j["maxUsers"];
    req.questionCount = j["questionCount"];
    req.answerTimeout = j["answerTimeout"];

    return req;
}