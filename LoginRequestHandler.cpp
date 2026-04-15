#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h" 
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"


constexpr int LOGIN_REQUEST_ID = 202;
constexpr int SIGNUP_REQUEST_ID = 33;
constexpr int LOGIN_SUCCESS_STATUS = 1;
constexpr int SIGNUP_SUCCESS_STATUS = 1;


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) : m_handlerFactory(factory)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    return (info.id == LOGIN_REQUEST_ID || info.id == SIGNUP_REQUEST_ID);
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info)
{
    RequestResult result;
    result.newHandler = nullptr;

    try
    {
        if (info.id == LOGIN_REQUEST_ID) // LOGIN
        {
            return login(info); 
        }
        else if (info.id == SIGNUP_REQUEST_ID) // SIGNUP
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



RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
    RequestResult result;
    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);

    LoginStatus status = m_handlerFactory.getLoginManager().login(req.username, req.password);

    LoginResponse res;
    if (status.status == LOGIN_SUCCESS_STATUS) // SUCCESS 
    {
        res.status = LOGIN_SUCCESS_STATUS;
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

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    RequestResult result;
    SignupRequest req = JsonRequestPacketDeserializer::deserializeSignUpRequest(info.buffer);

    SignUpStatus status = m_handlerFactory.getLoginManager().signup(req.username, req.password, req.email);

    SignupResponse res;
    if (status.status == SIGNUP_SUCCESS_STATUS) // SUCCESS
    {
        res.status = SIGNUP_SUCCESS_STATUS;
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