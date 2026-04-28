#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
    MenuRequestHandler(RequestHandlerFactory& factory, std::string user);
    ~MenuRequestHandler() = default;

    bool isRequestRelevant(const RequestInfo& info) const override;
    RequestResult handleRequest(const RequestInfo& info) override;
private:
	RequestHandlerFactory& m_handlerFactory;
    LoggedUser m_user;
    RequestResult signout(const RequestInfo& info);
    RequestResult getRooms(const RequestInfo& info);
    RequestResult getPlayersInRoom(const RequestInfo& info);
    RequestResult getPersonalStats(const RequestInfo& info);
    RequestResult getHighScore(const RequestInfo& info);
    RequestResult joinRoom(const RequestInfo& info);
    RequestResult createRoom(const RequestInfo& info);

};