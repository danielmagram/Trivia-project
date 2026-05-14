#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <unordered_map>
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
    std::unordered_map<SOCKET, std::unique_ptr<IRequestHandler>> m_clients;
    std::mutex m_clientsMutex;
    RequestHandlerFactory* m_handlerFactory;
};