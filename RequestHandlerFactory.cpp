#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "SqliteDatabase.h"

RequestHandlerFactory::RequestHandlerFactory()
    : m_database(new SqliteDatabase()), m_loginManager(m_database), m_statisticsManager(m_database)
{
}

RequestHandlerFactory::~RequestHandlerFactory()
{
    if (m_database != nullptr)
    {
        delete m_database;
    }
}

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this); // Interface instead of concrete class
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const std::string& user)
{
    return new MenuRequestHandler(*this, user); // fix this
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