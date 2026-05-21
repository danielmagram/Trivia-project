#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "LoggedUser.h"
#include "constants.h"

class RequestHandlerFactory;

class RoomRequestHandler : public IRequestHandler
{
public:
    RoomRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room);

    // Shared functionality
    RequestResult getRoomState(const RequestInfo& info) const;

protected:
    // Protected so that Admin and Member handlers can access them
    RequestHandlerFactory& m_handlerFactory;
    RoomManager& m_roomManager;
    LoggedUser m_user;
    Room* m_room;
};