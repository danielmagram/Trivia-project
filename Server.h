#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "IRequestHandler.h"

class Server
{
public:
	Server();
	~Server();
	void run();

private:

	void acceptClient();
	void clientHandler(SOCKET clientSocket);

	SOCKET _serverSocket;
	std::map<SOCKET, IRequestHandler*> _clients;
};

