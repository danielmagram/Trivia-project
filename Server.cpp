#include "Server.h"

Server::Server() : m_communicator(&m_handlerFactory)
{
}

void Server::run()
{
    m_communicator.startHandleRequests();
}