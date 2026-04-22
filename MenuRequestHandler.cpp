#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory) : m_handlerFactory(factory)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
    return RequestResult();
}
