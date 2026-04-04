#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "IRequestHandler.h"

class Communicator
{
public:
    Communicator();
    ~Communicator();

    void startHandleRequests();

private:
    void bindAndListen();
    void handleNewClient(SOCKET clientSocket);

    SOCKET m_serverSocket;
    std::map<SOCKET, IRequestHandler*> m_clients;
};