#pragma once
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <map>
#include <utility>

#pragma comment(lib, "ws2_32.lib")

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