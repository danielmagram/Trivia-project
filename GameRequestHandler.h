#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "GameManager.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
public:
    GameRequestHandler(RequestHandlerFactory& factory, LoggedUser user, int gameId);
    ~GameRequestHandler() = default;

	bool isRequestRelevant(const RequestInfo& info) const override;
    RequestResult handleRequest(const RequestInfo& info) override;
	void onClientDisconnected() override;
private:
    int m_gameId;
    LoggedUser m_user;
    GameManager& m_gameManager;
    RequestHandlerFactory& m_handlerFactory;

    // Game-specific request processors
    RequestResult getQuestion(const RequestInfo& info);
    RequestResult submitAnswer(const RequestInfo& info);
    RequestResult getGameResults(const RequestInfo& info);
    RequestResult leaveGame(const RequestInfo& info);
};