#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RoomManager.h"
#include "Constants.h"



MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory, std::string user) : m_handlerFactory(factory), m_user(user)
{

}



bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    RequestCode code = static_cast<RequestCode>(info.id);
    switch (code)
    {
    case RequestCode::GET_ROOMS:
    case RequestCode::GET_PLAYERS:
    case RequestCode::JOIN_ROOM:
    case RequestCode::CREATE_ROOM:
    case RequestCode::GET_HIGHSCORE:
    case RequestCode::GET_PERSONAL_STATS:
        return true;
    default:
        return false;
    }
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
	RequestCode code = static_cast<RequestCode>(info.id);
   switch (code)
    {
    case RequestCode::LOGOUT:
        return signout(info);

    case RequestCode::JOIN_ROOM:
        return joinRoom(info);

    case RequestCode::CREATE_ROOM:
        return createRoom(info);

    case RequestCode::GET_ROOMS:
        return getRooms(info);

    case RequestCode::GET_PLAYERS:
        return getPlayersInRoom(info);

    case RequestCode::GET_HIGHSCORE:
        return getHighScore(info);

    case RequestCode::GET_PERSONAL_STATS:
        return getPersonalStats(info);

    default:
        return RequestResult();
    }

    return RequestResult();
}

void MenuRequestHandler::onClientDisconnected()
{
    m_handlerFactory.getLoginManager().logout(m_user.getUsername());
}

RequestResult MenuRequestHandler::signout(const RequestInfo& info)
{
    LoginManager loginManager = m_handlerFactory.getLoginManager();
    LogoutResponse response;
	response.status = static_cast<unsigned int>(Status::SUCCESS);    
    loginManager.logout(m_user.getUsername());
    RequestResult result;
    
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createLoginRequestHandler();
    return result; // there is no info to return in the buffer just change the handler
}
RequestResult MenuRequestHandler::getRooms(const RequestInfo& info)
{
    RequestResult result;
    GetRoomsResponse response;
    response.rooms = m_handlerFactory.getRoomManager().getRooms();
	response.status = response.rooms.empty() ? static_cast<unsigned int>(Status::NO_ROOMS) : static_cast<unsigned int>(Status::SUCCESS);
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}
RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& info)
{
    RequestResult result;
    GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);
    Room* room = m_handlerFactory.getRoomManager().getRoomById(request.roomId);
    if (room == nullptr) {
        ErrorResponse err;
        err.message = "Room not found";
        result.response = JsonResponsePacketSerializer::serializeResponse(err);
        return result;
	}
    GetPlayersInRoomResponse response;
    std::vector<LoggedUser> players = room->getAllUsers();
    response.players.reserve(players.size()); // more efficient

    for (const auto& obj : players) {
        response.players.push_back(obj.getUsername());
    }
    
    result.response = JsonResponsePacketSerializer::serializeResponse(response);

    return result;
}
RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& info)
{
    GetPersonalStatsResponse response;
    response.statistics = m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getUsername());
	response.status = static_cast<unsigned int>(Status::SUCCESS);
    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}
RequestResult MenuRequestHandler::getHighScore(const RequestInfo& info)
{
    GetHighScoreResponse response;
    response.statistics = m_handlerFactory.getStatisticsManager().getHighScore();
	response.status = static_cast<unsigned int>(Status::SUCCESS);
    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}
RequestResult MenuRequestHandler::joinRoom(const RequestInfo& info)
{
    RequestResult result;
    JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
    Room* room = m_handlerFactory.getRoomManager().getRoomById(request.roomId);

    JoinRoomResponse response;
    if (room != nullptr) 
    {
		int maxPlayers = room->getMetadata().maxPlayers;
		std::vector<LoggedUser> players = room->getAllUsers();
		int countPlayers = static_cast<int>(players.size());
		if (countPlayers < maxPlayers) {
			room->addUser(m_user);
			response.status = static_cast<unsigned int>(Status::SUCCESS);
			result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, room);
		}
		else {
            response.status = static_cast<unsigned int>(Status::ROOM_FULL);
            result.response = JsonResponsePacketSerializer::serializeResponse(response);
            result.newHandler = nullptr;
            return result;
		}
	}
	else 
	{
		
        response.status = static_cast<unsigned int>(Status::ROOM_NOT_FOUND);
        result.response = JsonResponsePacketSerializer::serializeResponse(response);
        result.newHandler = nullptr;
        return result;
    }
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}


RequestResult MenuRequestHandler::createRoom(const RequestInfo& info)
{
    RequestResult result;
    CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
    RoomData data;
    data.name = request.roomName;
    data.maxPlayers = request.maxUsers;
    data.numOfQuestionsInGame = request.questionCount;
    data.timePerQuestion = request.answerTimeout;
	data.status = 0; // 0 - waiting for players, 1 - active
    m_handlerFactory.getRoomManager().createRoom(m_user, data);

    
    CreateRoomResponse response;
    response.status = static_cast<unsigned int>(Status::SUCCESS);

    result.response = JsonResponsePacketSerializer::serializeResponse(response);
	result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, m_handlerFactory.getRoomManager().getRoomById(data.id));
    return result;
}