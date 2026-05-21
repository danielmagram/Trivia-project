#pragma once
#include "RoomRequestHandler.h" 
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RoomMemberRequestHandler : public RoomRequestHandler
{
public:
    RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room);

    bool isRequestRelevant(const RequestInfo& info) const override;
    RequestResult handleRequest(const RequestInfo& info) override;
    void onClientDisconnected() override;

    RequestResult leaveRoom(const RequestInfo& info);
    RequestResult adminClosedRoom();

};