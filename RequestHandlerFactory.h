#pragma once
#include "LoginManager.h" 
#include "IDatabase.h"   
#include "IRequestHandler.h" 
class RequestHandlerFactory
{
public:
    RequestHandlerFactory();
    ~RequestHandlerFactory();

    // The return types must be the Base Interface!
    IRequestHandler* createLoginRequestHandler();
    IRequestHandler* createMenuRequestHandler();

    LoginManager& getLoginManager();

private:
    IDatabase* m_database;
    LoginManager m_loginManager;
};