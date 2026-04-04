#include "Communicator.h"
#include "LoginRequestHandler.h"
#include <iostream>
#include <thread>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

Communicator::Communicator()
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw std::exception("WSAStartup failed");
    }

    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
    {
        std::cout << "Error code: " << WSAGetLastError() << std::endl;
        throw std::exception("socket() failed");
    }
}

Communicator::~Communicator()
{
    try
    {
        closesocket(m_serverSocket);
        WSACleanup();
    }
    catch (...) {}
}

void Communicator::startHandleRequests()
{
    bindAndListen();

    std::thread([this]() {
        std::string input;
        while (true)
        {
            std::getline(std::cin, input);

            if (input == "EXIT")
            {
                std::cout << "Shutting down server..." << std::endl;
                closesocket(m_serverSocket);
                exit(0);
            }
        }
        }).detach();

    while (true)
    {
        std::cout << "Waiting for client..." << std::endl;

        SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cout << "accept failed" << WSAGetLastError() << std::endl;
            continue;
        }
        std::cout << "Client connected!" << std::endl;

        IRequestHandler* handler = new LoginRequestHandler();
        m_clients[clientSocket] = handler;

        std::thread(&Communicator::handleNewClient, this, clientSocket).detach();
    }
}

void Communicator::bindAndListen()
{
    int port = 1111;

    sockaddr_in sa = { 0 };
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
    {
        throw std::exception("bind() failed");
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        throw std::exception("listen() failed");
    }

    std::cout << "Listening on port " << port << std::endl;
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
    try
    {
        std::string msg = "Hello";
        send(clientSocket, msg.c_str(), msg.size(), 0);

        char buffer[6] = { 0 };
        recv(clientSocket, buffer, 5, 0);

        std::cout << "Client said: " << buffer << std::endl;

        closesocket(clientSocket);

        delete m_clients[clientSocket];
        m_clients.erase(clientSocket);
    }
    catch (...)
    {
        closesocket(clientSocket);

        delete m_clients[clientSocket];
        m_clients.erase(clientSocket);
    }
}