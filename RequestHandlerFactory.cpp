#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "SqliteDatabase.h"

RequestHandlerFactory::RequestHandlerFactory()
	: m_database(new SqliteDatabase()), m_loginManager(m_database), m_statisticsManager(m_database), m_GameManager(m_database)
{
}

RequestHandlerFactory::~RequestHandlerFactory()
{
    if (m_database != nullptr)
    {
        delete m_database;
    }
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::make_unique<LoginRequestHandler>(*this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const std::string& user)
{
    return std::make_unique<MenuRequestHandler>(*this, user);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser loggedUser, Room* room)
{
	return std::make_unique<RoomAdminRequestHandler>(*this, loggedUser, room);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser loggedUser, Room* room)
{
	return std::make_unique<RoomMemberRequestHandler>(*this, loggedUser, room);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return m_roomManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return m_statisticsManager;
}

GameManager& RequestHandlerFactory::getGameManager()
{
    return m_GameManager;
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createGameRequestHandler(LoggedUser loggedUser, int gameId)
{
    // This will return your GameRequestHandler once you write its skeleton class
    return std::make_unique<GameRequestHandler>(*this, loggedUser, gameId);
}