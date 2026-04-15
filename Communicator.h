#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <mutex>
#include "IRequestHandler.h"

class RequestHandlerFactory;

class Communicator
{
public:
    Communicator(RequestHandlerFactory* factory); 
    ~Communicator();

    void startHandleRequests();

private:
    void bindAndListen();
    void handleNewClient(SOCKET clientSocket);

    SOCKET m_serverSocket;
    std::map<SOCKET, IRequestHandler*> m_clients;
    std::mutex m_clientsMutex; 
    RequestHandlerFactory* m_handlerFactory;
};