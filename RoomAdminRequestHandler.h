#pragma once
#include "RoomRequestHandler.h" 

class RoomAdminRequestHandler : public RoomRequestHandler
{
public:
    RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room);

    bool isRequestRelevant(const RequestInfo& info) const override;
    RequestResult handleRequest(const RequestInfo& info) override;
    void onClientDisconnected() override;

    RequestResult closeRoom(const RequestInfo& info);
    RequestResult startGame(const RequestInfo& info);

};