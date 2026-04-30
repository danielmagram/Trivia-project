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

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler()
{
    return std::make_unique<LoginRequestHandler>(*this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const std::string& user)
{
    return std::make_unique<MenuRequestHandler>(*this, user);
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