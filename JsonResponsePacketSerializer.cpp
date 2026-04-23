#include "JsonResponsePacketSerializer.h"
#include "json.hpp" 
using json = nlohmann::json;

constexpr unsigned char ERROR_CODE = 40;
constexpr unsigned char LOGIN_CODE = 202;
constexpr unsigned char SIGNUP_CODE = 33;
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