#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h" 
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "constants.h"



LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) : m_handlerFactory(factory)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    RequestCode code = static_cast<RequestCode>(info.id);
    return (code == RequestCode::LOGIN || code == RequestCode::SIGNUP);
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info)
{
    RequestResult result;
    result.newHandler = nullptr;

    try
    {
        if (info.id == static_cast<unsigned char>(RequestCode::LOGIN)) // LOGIN
        {
            return login(info); 
        }
        else if (info.id == static_cast<unsigned char>(RequestCode::SIGNUP)) // SIGNUP
        {
            return signup(info); 
        }
    }
    catch (...)
    {
        ErrorResponse err;
        err.message = "An error occurred";
        result.response = JsonResponsePacketSerializer::serializeResponse(err);
        result.newHandler = nullptr;
    }

    return result;
}

void LoginRequestHandler::onClientDisconnected()
{
}



RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
    RequestResult result;
    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);

    LoginStatus status = m_handlerFactory.getLoginManager().login(req.username, req.password);

    LoginResponse res;
    if (status.status == static_cast<unsigned int>(Status::SUCCESS)) // SUCCESS
    {
		res.status = static_cast<unsigned int>(Status::SUCCESS);
        result.response = JsonResponsePacketSerializer::serializeResponse(res);
        result.newHandler = m_handlerFactory.createMenuRequestHandler(req.username);
    }
    else
    {
		res.status = status.status; // Pass the specific error code back to the client
        result.response = JsonResponsePacketSerializer::serializeResponse(res);
        result.newHandler = m_handlerFactory.createLoginRequestHandler();
    }
    return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    RequestResult result;
    SignupRequest req = JsonRequestPacketDeserializer::deserializeSignUpRequest(info.buffer);

    SignUpStatus status = m_handlerFactory.getLoginManager().signup(req.username, req.password, req.email);

    SignupResponse res;
    if (status.status == static_cast<unsigned int>(Status::SUCCESS)) // SUCCESS
    {
        res.status = static_cast<unsigned int>(Status::SUCCESS);
        result.response = JsonResponsePacketSerializer::serializeResponse(res);
        result.newHandler = m_handlerFactory.createMenuRequestHandler(req.username);
    }
    else
    {
        res.status = status.status;
        result.response = JsonResponsePacketSerializer::serializeResponse(res);
        result.newHandler = m_handlerFactory.createLoginRequestHandler();
    }
    return result;
}