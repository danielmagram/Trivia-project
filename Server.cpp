//irequest handler
//login request handler
#include "Server.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>

Server::Server()
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		throw std::exception("WSAStartup failed");
	}

	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_serverSocket == INVALID_SOCKET)
	{
		std::cout << "Error code: " << WSAGetLastError() << std::endl;
		throw std::exception("socket()");
	}
}

Server::~Server()
{
	try
	{
		closesocket(_serverSocket);
		WSACleanup();
	}
	catch (...) {}
}
void Server::run()
{
	int port = 1111;
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception("bind()");

	// Start listening for incoming requests of clients
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception("listen()");
	std::cout << "Listening on port " << port << std::endl;

	std::thread([this]() {
		std::string input;
		while (true)
		{
			std::cin >> input;
			if (input == "EXIT")
			{
				std::cout << "Shutting down server..." << std::endl;
				closesocket(_serverSocket);
				exit(0);
			}
		}
		}).detach();

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		acceptClient();
	}
}


void Server::acceptClient()
{

	// this accepts the client and create a specific socket from server to this client
	// the process will not continue until a client connects to the server
	SOCKET client_socket = accept(_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception("accept()");

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	// the function that handle the conversation with the client
	IRequestHandler* handler = new LoginRequestHandler();
	_clients[client_socket] = handler;
	std::thread(&Server::clientHandler, this, client_socket).detach();

}


void Server::clientHandler(SOCKET clientSocket)
{
	try
	{
		std::string s = "Hello";
		char m[6] = { 0 };
		while (true)
		{
			send(clientSocket, s.c_str(), s.size(), 0);
			recv(clientSocket, m, 5, 0);
			std::cout << m << std::endl;

		}

		closesocket(clientSocket);

		delete _clients[clientSocket];
		_clients.erase(clientSocket);
	}
	catch (const std::exception& e)
	{
		closesocket(clientSocket);

		delete _clients[clientSocket];
		_clients.erase(clientSocket);
	}


}



int main()
{
	try
	{
		Server s;
		s.run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}