#pragma once
#include "IRequestHandler.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
    MenuRequestHandler(RequestHandlerFactory& factory);
    ~MenuRequestHandler() = default;

    bool isRequestRelevant(const RequestInfo& info) const override;
    RequestResult handleRequest(const RequestInfo& info) override;
private:
	RequestHandlerFactory& m_handlerFactory;
};