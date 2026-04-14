#pragma once
#include "IRequestHandler.h"

class RequestHandlerFactory; // Forward declaration

class LoginRequestHandler : public IRequestHandler
{
public:
    LoginRequestHandler(RequestHandlerFactory& factory);
    ~LoginRequestHandler() = default;

    bool isRequestRelevant(RequestInfo info) override;
    RequestResult handleRequest(RequestInfo info) override;

private:
    RequestHandlerFactory& m_handlerFactory;

    RequestResult login(RequestInfo info);
    RequestResult signup(RequestInfo info);
};