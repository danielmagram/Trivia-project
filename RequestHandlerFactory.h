#pragma once
#include "LoginManager.h" 
#include "IDatabase.h"   

// Forward declarations - because of circular dependencies between the factory and the handlers
class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
public:
    RequestHandlerFactory();
    ~RequestHandlerFactory();

    LoginRequestHandler* createLoginRequestHandler();
    MenuRequestHandler* createMenuRequestHandler();

    LoginManager& getLoginManager();

private:
    LoginManager m_loginManager;
    IDatabase* m_database;
};