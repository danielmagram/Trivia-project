#pragma once
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "LoggedUser.h"
#include "constants.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RequestHandlerFactory;

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room);
	bool isRequestRelevant(const RequestInfo& info) const override;
	RequestResult handleRequest(const RequestInfo& info) override;
	RequestResult getRoomState(const RequestInfo& info) const;
	void onClientDisconnected() override;
	RequestResult leaveRoom(const RequestInfo& info);
private:
	RequestHandlerFactory& m_handlerFactory;
	RoomManager& m_roomManager;
	LoggedUser m_user;
	Room* m_room;
};