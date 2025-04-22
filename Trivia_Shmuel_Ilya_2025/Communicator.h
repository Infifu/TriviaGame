#pragma once
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <map>
#include <utility>
#include <vector>

typedef std::vector<unsigned char> Buffer;
#pragma comment(lib, "ws2_32.lib")

class Communicator
{
public:
	Communicator();
	Communicator(IDatabase* database);
	void startHandleRequests();

private:
	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);

	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	IDatabase* m_database;
};