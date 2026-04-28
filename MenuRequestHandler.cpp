#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RoomManager.h"

enum CODES : unsigned char {
    ERROR_CODE = 40,
    LOGIN_CODE = 202,
    SIGNUP_CODE = 33,
    SIGNOUT_CODE = 203,
    JOIN_ROOM_CODE = 100,
    CREATE_ROOM_CODE = 101,
    GET_ROOMS_CODE = 102,
    GET_HIGHSCORE_CODE = 140,
    GET_PERSONAL_STATS_CODE = 150,
    GET_PLAYERS_CODE = 105,
};


MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory, std::string user) : m_handlerFactory(factory), m_user(user)
{

}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    switch (info.id)
    {
    case GET_ROOMS_CODE:
    case GET_PLAYERS_CODE:
    case JOIN_ROOM_CODE:
    case CREATE_ROOM_CODE:
    case GET_HIGHSCORE_CODE:
    case GET_PERSONAL_STATS_CODE:
        return true;

    default:
        return false;
    }
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
    switch (info.id)
    {
    case SIGNOUT_CODE:
        return signout(info);

    case JOIN_ROOM_CODE:
        return joinRoom(info);
    
    case CREATE_ROOM_CODE:
        return createRoom(info);

    case GET_ROOMS_CODE:
        return getRooms(info);

    case GET_PLAYERS_CODE:
        return getPlayersInRoom(info);

    case GET_HIGHSCORE_CODE:
        return getHighScore(info);

    case GET_PERSONAL_STATS_CODE:
        return getPersonalStats(info);

    default:
        return RequestResult();
    }

    return RequestResult();
}

RequestResult MenuRequestHandler::signout(const RequestInfo& info)
{
    LoginManager loginManager = m_handlerFactory.getLoginManager();
    LogoutResponse response;
    response.status = 1;
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
    response.status = (response.rooms.empty()) ? 1 : 0;
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}
RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& info)
{
    RequestResult result;
    GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(info.buffer);
    Room* room = m_handlerFactory.getRoomManager().getRoomById(request.roomId);
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
    response.status = (response.statistics.empty()) ? 1 : 0;
    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}
RequestResult MenuRequestHandler::getHighScore(const RequestInfo& info)
{
    GetHighScoreResponse response;
    response.statistics = m_handlerFactory.getStatisticsManager().getHighScore();
    response.status = (response.statistics.empty()) ? 1 : 0;
    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}
RequestResult MenuRequestHandler::joinRoom(const RequestInfo& info)
{
    RequestResult result;
    JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
    Room* room = m_handlerFactory.getRoomManager().getRoomById(request.roomId);
    room->addUser(m_user);
    JoinRoomResponse response;
    response.status = 1;

    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}
RequestResult MenuRequestHandler::createRoom(const RequestInfo& info)
{
    RequestResult result;
    CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
    RoomData data;
    data.id;
    data.name = request.roomName;
    data.maxPlayers = request.maxUsers;
    data.numOfQuestionsInGame = request.questionCount;
    data.timePerQuestion = request.answerTimeout;
    data.status = 0;
    m_handlerFactory.getRoomManager().createRoom(m_user, data);

    
    CreateRoomResponse response;
    response.status = 1;

    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    return result;
}