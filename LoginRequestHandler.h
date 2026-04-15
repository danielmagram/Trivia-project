#pragma once
#include "IRequestHandler.h"

class RequestHandlerFactory; // Forward declaration

class LoginRequestHandler : public IRequestHandler
{
public:
    LoginRequestHandler(RequestHandlerFactory& factory);
    ~LoginRequestHandler() = default;

    bool isRequestRelevant(const RequestInfo& info) const override;
    RequestResult handleRequest(const RequestInfo& info) override;

private:
    RequestHandlerFactory& m_handlerFactory;

    RequestResult login(const RequestInfo& info);
    RequestResult signup(const RequestInfo& info);
};