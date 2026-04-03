#include "Server.h"

void Server::run()
{
    m_communicator.startHandleRequests();
}