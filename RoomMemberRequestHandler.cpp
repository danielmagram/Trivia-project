#include "RoomMemberRequestHandler.h"


RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room) 
    : m_handlerFactory(factory), m_user(user), m_room(room), m_roomManager(factory.getRoomManager())
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    RequestCode code = static_cast<RequestCode>(info.id);
	return (code == RequestCode::GET_ROOM_STATE || code == RequestCode::LEAVE_ROOM);
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& info)
{
    RequestResult result;
    result.newHandler = nullptr;
    try
    {
        if (info.id == static_cast<unsigned char>(RequestCode::GET_ROOM_STATE)) // GET_ROOM_STATE
        {
            return getRoomState(info); 
        }
        else if (info.id == static_cast<unsigned char>(RequestCode::LEAVE_ROOM)) // LEAVE_ROOM
        {
            return leaveRoom(info); 
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

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& info) const
{
    RequestResult result;
    GetRoomStateResponse response;
	// get the room state from the room manager
    Room* room = m_handlerFactory.getRoomManager().getRoomById(m_room->getMetadata().id);
    if (room == nullptr) {
		response.status = static_cast<unsigned int>(Status::ROOM_NOT_FOUND);
        result.response = JsonResponsePacketSerializer::serializeResponse(response);
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

void RoomMemberRequestHandler::onClientDisconnected()
{
    m_room->removeUser(m_user.getUsername());
	m_handlerFactory.getLoginManager().logout(m_user.getUsername());
	
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& info)
{
    RequestResult result;
    result.newHandler = nullptr;
    try
    {
        if (info.id == static_cast<unsigned char>(RequestCode::LEAVE_ROOM))
        {
			// Remove the user from the room
            m_room->removeUser(m_user.getUsername());
            
            LeaveRoomResponse response;
            response.status = static_cast<unsigned int>(Status::SUCCESS);
            result.response = JsonResponsePacketSerializer::serializeResponse(response);
			result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
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
