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

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this); // Interface instead of concrete class
}

IRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
    return new MenuRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return m_loginManager;
}