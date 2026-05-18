#pragma once
#include "RoomAdminRequestHandler.h"
#include "jsonResponsePacketSerializer.h"


RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room) 
	: m_handlerFactory(factory), m_user(user), m_room(room), m_roomManager(factory.getRoomManager())
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
	RequestCode code = static_cast<RequestCode>(info.id);
	return (code == RequestCode::GET_ROOM_STATE || code == RequestCode::CLOSE_ROOM || code == RequestCode::START_GAME);
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& info)
{
	RequestResult result;
	result.newHandler = nullptr;
	try
	{
		if (info.id == static_cast<unsigned char>(RequestCode::GET_ROOM_STATE)) // GET_ROOM_STATE
		{
			return getRoomState(info); 
		}
		else if (info.id == static_cast<unsigned char>(RequestCode::CLOSE_ROOM)) // CLOSE_ROOM
		{
			return closeRoom(info); 
		}
		else if (info.id == static_cast<unsigned char>(RequestCode::START_GAME)) // START_GAME
		{
			return startGame(info); 
		}
	}
	catch (...)
	{
		ErrorResponse err;
		err.message = "An error occurred";
		result.response = JsonResponsePacketSerializer::serializeResponse(err);
		result.newHandler = nullptr;
	}
	return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& info) const
{
	RequestResult result;
	GetRoomStateResponse response;
	// get the room state from the room manager
	Room* room = m_handlerFactory.getRoomManager().getRoomById(m_room->getMetadata().id);
	if (room == nullptr) {
		ErrorResponse err;
		err.message = "Room not found";
		result.response = JsonResponsePacketSerializer::serializeResponse(err);
		return result;
	}

	response.status = static_cast<unsigned int>(Status::SUCCESS);
	response.hasGameBegun = room->getMetadata().status == 1; // 1 - mean active, 0 - waiting for players
	response.players = room->getAllUsers();
	response.questionCount = room->getMetadata().numOfQuestionsInGame;
	response.answerTimeout = static_cast<float>(room->getMetadata().timePerQuestion);

	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	return result;
}

void RoomAdminRequestHandler::onClientDisconnected()
{
	m_handlerFactory.getRoomManager().deleteRoom(m_room->getMetadata().id);
	m_handlerFactory.getLoginManager().logout(m_user.getUsername());
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& info)
{
	RequestResult result;
	CloseRoomResponse response;
	m_handlerFactory.getRoomManager().deleteRoom(m_room->getMetadata().id);
	response.status = static_cast<unsigned int>(Status::SUCCESS);
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
	return result;
}


RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& info)
{
	RequestResult result;
	StartGameResponse response;
	Room* room = m_handlerFactory.getRoomManager().getRoomById(m_room->getMetadata().id);
	if (room == nullptr) {
		ErrorResponse err;
		err.message = "Room not found";
		result.response = JsonResponsePacketSerializer::serializeResponse(err);
		return result;
	}
	room->getMetadata().status = 1; 
	response.status = static_cast<unsigned int>(Status::SUCCESS);
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	return result;
}