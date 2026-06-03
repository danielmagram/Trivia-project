#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "Constants.h"

GameRequestHandler::GameRequestHandler(RequestHandlerFactory& factory, LoggedUser user, int gameId)
    : m_handlerFactory(factory), m_user(user), m_gameId(gameId), m_gameManager(factory.getGameManager())
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    return info.id == static_cast<unsigned char>(RequestCode::GET_QUESTION) ||
        info.id == static_cast<unsigned char>(RequestCode::SUBMIT_ANSWER) ||
        info.id == static_cast<unsigned char>(RequestCode::GET_GAME_RESULTS) ||
        info.id == static_cast<unsigned char>(RequestCode::LEAVE_GAME);
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& info)
{
	unsigned char code = static_cast<unsigned char>(info.id);
    switch (code)
    {
    case static_cast<unsigned char>(RequestCode::GET_QUESTION):
        return getQuestion(info);
    case static_cast<unsigned char>(RequestCode::SUBMIT_ANSWER):
        return submitAnswer(info);
    case static_cast<unsigned char>(RequestCode::GET_GAME_RESULTS):
        return getGameResults(info);
    case static_cast<unsigned char>(RequestCode::LEAVE_GAME):
        return leaveGame(info);
    default:
        ErrorResponse err{ "Invalid game request code" };
        return RequestResult{ JsonResponsePacketSerializer::serializeResponse(err), nullptr };
    }
}
void GameRequestHandler::onClientDisconnected()
{
    try
    {
        Game& activeGame = m_gameManager.getGameById(m_gameId);
        activeGame.removePlayer(m_user);
		m_handlerFactory.getLoginManager().logout(m_user.getUsername());


    }
    catch (...)
    {
		// If the game or player is not found, we can ignore it since the client is already disconnected
    }
}
RequestResult GameRequestHandler::getQuestion(const RequestInfo& info)
{
    RequestResult result;
    GetQuestionResponse response;

    try
    {
		Game& activeGame = m_gameManager.getGameById(m_gameId);
        Question q = activeGame.getQuestionForUser(m_user);

        response.status = static_cast<unsigned int>(Status::SUCCESS);
        response.question = q.getQuestion();

        std::vector<std::string> possibleAnswers = q.getAnswers();
        for (unsigned int i = 0; i < possibleAnswers.size(); i++)
        {
            response.answers[i] = possibleAnswers[i];
        }
    }
    catch (...)
    {
        response.status = static_cast<unsigned int>(Status::GENERIC_ERROR);
    }

    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr; 
    return result;
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& info)
{
    RequestResult result;
    SubmitAnswerResponse response;

    try
    {
        SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
        Game& activeGame = m_gameManager.getGameById(m_gameId);

        Question q = activeGame.getCurrentQuestion(m_user);
		std::vector<std::string> possibleAnswers = q.getAnswers();

        std::string selectedAnswerText = "";
        if (request.answerId >= 0 && request.answerId < possibleAnswers.size()) {
            selectedAnswerText = possibleAnswers[request.answerId];
        }

        unsigned int correctId = 0;
        for (unsigned int i = 0; i < possibleAnswers.size(); i++) {
            if (possibleAnswers[i] == q.getCorrectAnswer()) {
                correctId = i;
                break;
            }
        }

        activeGame.submitAnswer(selectedAnswerText, m_user);

        response.status = static_cast<unsigned int>(Status::SUCCESS);
        response.correctAnswerId = correctId;
    }
    catch (...)
    {
        response.status = static_cast<unsigned int>(Status::GENERIC_ERROR);
    }

    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr; 
    return result;
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo& info)
{
    RequestResult result;
    GetGameResultsResponse response;
    if (m_gameManager.getGameById(m_gameId).isGameFinished())
    {
        try
        {
            Game& activeGame = m_gameManager.getGameById(m_gameId);
            std::map<LoggedUser, GameData, UserCompare> players = activeGame.getPlayers();

            for (const auto& pair : players)
            {
                PlayerResults pResult;
                pResult.username = pair.first.getUsername();
                pResult.correctAnswerCount = pair.second.correctAnswerCount;
                pResult.wrongAnswerCount = pair.second.totalAnswerCount - pair.second.correctAnswerCount;
                pResult.averageAnswerTime = pair.second.avrageAnswerTime;

                response.results.push_back(pResult);
            }

            response.status = static_cast<unsigned int>(Status::SUCCESS);
        }
        catch (...)
        {
            response.status = static_cast<unsigned int>(Status::GENERIC_ERROR);
        }
    }
    else
    {
        response.status = static_cast<unsigned int>(Status::GAME_NOT_FINISHED);
    }
    result.response = JsonResponsePacketSerializer::serializeResponse(response);

	result.newHandler = nullptr;
    return result;
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo& reqInfo)
{
    m_gameManager.removePlayer(m_gameId, m_user);

    RequestResult result;
	LeaveGameResponse response;
	response.status = static_cast<unsigned int>(Status::SUCCESS);
    result.response = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());

    return result;
}