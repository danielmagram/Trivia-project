#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "SqliteDatabase.h"

RequestHandlerFactory::RequestHandlerFactory()
    : m_database(new SqliteDatabase()), m_loginManager(m_database)
{
}

RequestHandlerFactory::~RequestHandlerFactory()
{
    if (m_database != nullptr)
    {
        delete m_database;
    }
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	// we pass the factory to the handler so it can create new handlers when needed, and access the login manager
    return new LoginRequestHandler(*this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
    return new MenuRequestHandler();
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}