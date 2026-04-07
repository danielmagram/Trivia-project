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