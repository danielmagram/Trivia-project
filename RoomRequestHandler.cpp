#include "RoomRequestHandler.h"
#include "jsonResponsePacketSerializer.h"

RoomRequestHandler::RoomRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room* room)
    : m_handlerFactory(factory), m_user(user), m_room(room), m_roomManager(factory.getRoomManager())
{
}

RequestResult RoomRequestHandler::getRoomState(const RequestInfo& info) const
{
    RequestResult result;
    GetRoomStateResponse response;
    Room* room = m_handlerFactory.getRoomManager().getRoomById(m_room->getMetadata().id);
    if (room == nullptr) {
        response.status = static_cast<unsigned int>(Status::ROOM_NOT_FOUND);
        result.response = JsonResponsePacketSerializer::serializeResponse(response);
        return result;
    }

    bool gameHasBegun = (room->getMetadata().status == 1);

    response.status = static_cast<unsigned int>(Status::SUCCESS);
    response.hasGameBegun = gameHasBegun;
    response.players = room->getAllUsers();
    response.questionCount = room->getMetadata().numOfQuestionsInGame;
    response.answerTimeout = static_cast<float>(room->getMetadata().timePerQuestion);

    result.response = JsonResponsePacketSerializer::serializeResponse(response);

    if (gameHasBegun) {
        int gameId = m_handlerFactory.getGameManager().getGameIdByUsername(m_user.getUsername());
        if (gameId != -1) {
            result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, gameId);
        }
    }

    return result;
}