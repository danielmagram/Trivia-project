#pragma once
#include <string>
#include <vector>


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


class JsonResponsePacketSerializer
{
public:
    static std::vector<unsigned char> serializeErrorResponse(ErrorResponse response);
    static std::vector<unsigned char> serializeLoginResponse(LoginResponse response);
    static std::vector<unsigned char> serializeSignupResponse(SignupResponse response);
};