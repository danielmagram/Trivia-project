#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

bool LoginRequestHandler::isRequestRelevant(RequestInfo info)
{
    // Check if the message code is Login (202) or SignUp (33)
    if (info.id == 202 || info.id == 33)
    {
        return true;
    }
    return false;
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo info)
{
    RequestResult result;

    if (info.id == 202) // LOGIN
    {
        // Deserialize the request
        LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);


        // Build the response
        LoginResponse res;
        res.status = 1; // 1 for success

        // Serialize the response
        result.response = JsonResponsePacketSerializer::serializeLoginResponse(res);

		result.newHandler = new LoginRequestHandler();// for now, we stay in the same handler, later it will change
    }
    else if (info.id == 33) // SIGNUP
    {
        SignupRequest req = JsonRequestPacketDeserializer::deserializeSignUpRequest(info.buffer);


        SignupResponse res;
        res.status = 1;

        result.response = JsonResponsePacketSerializer::serializeSignupResponse(res);
        result.newHandler = new LoginRequestHandler();
    }
    else
    {
        ErrorResponse res;
        res.message = "ERROR: Invalid Request";
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(res);
        result.newHandler = new LoginRequestHandler();
    }

    return result;
}