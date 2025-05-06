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
#include "UsingBuffer.h"

#pragma comment(lib, "ws2_32.lib")

class Communicator
{
public:
	Communicator(IDatabase* database, RequestHandlerFactory* handlerFactory);
	~Communicator();
	void startHandleRequests();

private:
	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
	static SOCKET socketInit();

	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	IDatabase* m_database;
};