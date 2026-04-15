#pragma once
#include "Communicator.h"
#include "RequestHandlerFactory.h"

class Server
{
public:
    Server();
    void run();

private:
    RequestHandlerFactory m_handlerFactory; 
    Communicator m_communicator;
};