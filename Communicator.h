#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <mutex> // Added for thread safety
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
    std::mutex m_clientsMutex; // The lock for our map
};