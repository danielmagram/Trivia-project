#pragma once
#include <string>
#include <vector>

struct LoginRequest
{
    std::string username;
    std::string password;
};

struct SignupRequest
{
    std::string username;
    std::string password;
    std::string email;
};

class JsonRequestPacketDeserializer
{
public:
    static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& buffer);
    static SignupRequest deserializeSignUpRequest(const std::vector<unsigned char>& buffer);
};