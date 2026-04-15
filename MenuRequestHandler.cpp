#include "MenuRequestHandler.h"

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    return false;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
    return RequestResult();
}
