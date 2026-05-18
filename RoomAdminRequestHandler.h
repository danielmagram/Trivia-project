#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "LoggedUser.h"
#include "constants.h"


class RequestHandlerFactory;

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room);
	bool isRequestRelevant(const RequestInfo& info) const override;
	RequestResult handleRequest(const RequestInfo& info) override;
	RequestResult getRoomState(const RequestInfo& info) const;
	void onClientDisconnected() override;
	RequestResult closeRoom(const RequestInfo& info);
	RequestResult startGame(const RequestInfo& info);
private:
	RequestHandlerFactory& m_handlerFactory;
	RoomManager& m_roomManager;
	LoggedUser m_user;
	Room* m_room;
};