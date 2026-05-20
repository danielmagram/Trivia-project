#include "RoomMemberRequestHandler.h"


RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room)
    : RoomRequestHandler(factory, user, room)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    RequestCode code = static_cast<RequestCode>(info.id);
    return (code == RequestCode::GET_ROOM_STATE || code == RequestCode::LEAVE_ROOM || code == RequestCode::ADMIN_CLOSED_ROOM);
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
        else if (info.id == static_cast<unsigned char>(RequestCode::ADMIN_CLOSED_ROOM)) // ADMIN_CLOSED_ROOM
        {
            return adminClosedRoom(); 
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

RequestResult RoomMemberRequestHandler::adminClosedRoom()
{
	// This function is called when the admin of the room closes it. It should notify the member that the room has been closed and return them to the menu.
	RequestResult result;
	CloseRoomResponse response;
	response.status = static_cast<unsigned int>(Status::ROOM_NOT_FOUND);
	result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
	return result;
}


