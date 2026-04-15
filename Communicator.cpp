#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include <iostream>
#include <thread>
#include <memory>
#include "JsonResponsePacketSerializer.h"
#include <ws2tcpip.h>
#include <cstring>
#include <chrono>


#pragma comment(lib, "Ws2_32.lib")

Communicator::Communicator(RequestHandlerFactory* factory) : m_handlerFactory(factory)
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

        IRequestHandler* handler = m_handlerFactory->createLoginRequestHandler();
        // --- LOCK BEFORE ADDING TO MAP ---
        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            m_clients[clientSocket] = handler;
        }
        // Lock automatically releases here

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
        // BUG FIX 3: We need an infinite loop so the client can send multiple messages!
        while (true)
        {
            char code;
            int received = recv(clientSocket, &code, 1, 0);

            // If received is 0, the client disconnected safely. If < 0, error.
            if (received <= 0) break;

            auto now = std::chrono::system_clock::now();
            std::time_t time = std::chrono::system_clock::to_time_t(now);

            char size[4];
            received = recv(clientSocket, size, 4, 0);

            // BUG FIX 1: We are expecting 4 bytes, so if it's <= 0, something broke.
            if (received <= 0) break;

            uint32_t netSize = 0;
            std::memcpy(&netSize, size, 4);
            int payloadSize = ntohl(netSize);

            std::vector<unsigned char> data(payloadSize);
            uint32_t total = 0;

            while (total < payloadSize)
            {
                int toRead = static_cast<int>(payloadSize - total);
                int got = recv(clientSocket, reinterpret_cast<char*>(data.data() + total), toRead, 0);
                if (got <= 0)
                {
                    throw std::runtime_error("recv data failed");
                }
                total += static_cast<uint32_t>(got);
            }

            RequestInfo info;
            info.buffer = data;
            info.id = code;
            info.receivalTime = time;

            // BUG FIX 4: Get the handler from the map, don't create a random new one!
            IRequestHandler* handler = nullptr;
            {
                std::lock_guard<std::mutex> lock(m_clientsMutex);
                handler = m_clients[clientSocket];
            }

            RequestResult result;

            if (handler->isRequestRelevant(info))
            {
                result = handler->handleRequest(info);
            }
            else
            {
                ErrorResponse res;
                res.message = "ERROR: Irrelevant Request";
                result.response = JsonResponsePacketSerializer::serializeErrorResponse(res);
                result.newHandler = handler; // Stay in current state
            }

            // BUG FIX 2: Only use ONE send loop! Do not double-send.
            size_t sentBytes = 0;
            while (sentBytes < result.response.size())
            {
                int s = send(clientSocket, reinterpret_cast<const char*>(result.response.data() + sentBytes),
                    static_cast<int>(result.response.size() - sentBytes), 0);
                if (s <= 0)
                {
                    throw std::runtime_error("sending response failed");
                }
                sentBytes += static_cast<size_t>(s);
            }

            // BUG FIX 5: Actually update the state machine instead of deleting the new handler
            if (result.newHandler != nullptr && result.newHandler != handler)
            {
                std::lock_guard<std::mutex> lock(m_clientsMutex);
                m_clients[clientSocket] = result.newHandler;
                delete handler; // Delete the OLD handler, we are now using the NEW one
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Client Thread Exception: " << e.what() << std::endl;
    }

    // We only reach here if the while(true) loop breaks (Client disconnected)
    closesocket(clientSocket);

    // Clean up the map
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        if (m_clients.find(clientSocket) != m_clients.end())
        {
            delete m_clients[clientSocket];
            m_clients.erase(clientSocket);
        }
    }
}

