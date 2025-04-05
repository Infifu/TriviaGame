#pragma once
#include "IRequestHandler.h"
#include <winsock.h>
#include <winsock2.h>
#include <map>

class Communicator {
public:
	Communicator();
	void startHandleRequests();

private:
	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);

	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
};