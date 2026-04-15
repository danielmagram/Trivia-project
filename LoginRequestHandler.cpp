#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h" 
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) : m_handlerFactory(factory)
{
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo info)
{
    return (info.id == 202 || info.id == 33);
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo info)
{
    RequestResult result;
    result.newHandler = nullptr;

    try
    {
        if (info.id == 202) // LOGIN
        {
            return login(info); 
        }
        else if (info.id == 33) // SIGNUP
        {
            return signup(info); 
        }
    }
    catch (...)
    {
        ErrorResponse err;
        err.message = "An error occurred";
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(err);
        result.newHandler = nullptr;
    }

    return result;
}



RequestResult LoginRequestHandler::login(RequestInfo info)
{
    RequestResult result;
    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);

    LoginStatus status = m_handlerFactory.getLoginManager().login(req.username, req.password);

    LoginResponse res;
    if (status.status == 1) // SUCCESS 
    {
        res.status = 1;
        result.response = JsonResponsePacketSerializer::serializeLoginResponse(res);
        result.newHandler = m_handlerFactory.createMenuRequestHandler();
    }
    else
    {
		res.status = status.status; // Pass the specific error code back to the client
        result.response = JsonResponsePacketSerializer::serializeLoginResponse(res);
        result.newHandler = m_handlerFactory.createLoginRequestHandler();
    }
    return result;
}

RequestResult LoginRequestHandler::signup(RequestInfo info)
{
    RequestResult result;
    SignupRequest req = JsonRequestPacketDeserializer::deserializeSignUpRequest(info.buffer);

    SignUpStatus status = m_handlerFactory.getLoginManager().signup(req.username, req.password, req.email);

    SignupResponse res;
    if (status.status == 1) // SUCCESS
    {
        res.status = 1;
        result.response = JsonResponsePacketSerializer::serializeSignupResponse(res);
        result.newHandler = m_handlerFactory.createMenuRequestHandler();
    }
    else
    {
        res.status = status.status;
        result.response = JsonResponsePacketSerializer::serializeSignupResponse(res);
        result.newHandler = m_handlerFactory.createLoginRequestHandler();
    }
    return result;
}