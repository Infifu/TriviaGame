#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "IDatabase.h"
#include "TriviaException.h"

#define PORT 6969

Communicator::Communicator(IDatabase* database, RequestHandlerFactory* handlerFactory) : m_handlerFactory(*handlerFactory), m_database(database), m_serverSocket(Communicator::socketInit()) {}

Communicator::~Communicator()
{
	try
	{
		closesocket(m_serverSocket);
	}
	catch (...) {}
}

void Communicator::startHandleRequests()
{
	bindAndListen();

	while (true)
	{
		SOCKET client_socket = accept(m_serverSocket, nullptr, nullptr);

		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "Client accepted. Server and client can speak" << std::endl;
		std::thread thread(&Communicator::handleNewClient, this, client_socket);
		thread.detach();
	}
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - bind");
	}

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::exception(__FUNCTION__ " - listen");
	}
	std::cout << "Listening on port " << PORT << std::endl;
}


//i think i forgot to mutex somewhere but idrc rn so whatever
void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{ 
		LoginRequestHandler* loginRequestHandler = m_handlerFactory.createLoginRequestHandler();
		m_clients.insert(std::pair<SOCKET, LoginRequestHandler*>(clientSocket, loginRequestHandler));

		unsigned char code;
		Buffer msgLength(4);

		int recieved = recv(clientSocket, reinterpret_cast<char*>(&code), 1, 0); //recieve the code
		if (recieved < 1)
		{
			throw std::exception("Recieve failed");
		}

		recieved = recv(clientSocket, reinterpret_cast<char*>(&msgLength[0]), 4, 0); //recieve the length
		if (recieved < 4)
		{
			throw std::exception("Recieve failed");
		}

		int length = 0;
		std::memcpy(&length, msgLength.data(), sizeof(length));

		Buffer buffer(length);
		recieved = recv(clientSocket, reinterpret_cast<char*>(&buffer[0]), length, 0); //recieve the rest of the message
		if (recieved < 5)
		{
			throw std::exception("Recieve failed");
		}

		RequestInfo requestinfo;
		requestinfo.id = code;
		requestinfo.buffer = buffer;
		requestinfo.receivalTime = std::time(nullptr);

		RequestResult requestResult = loginRequestHandler->handleRequest(requestinfo);
		m_clients.find(clientSocket)->second = requestResult.newHandler;

		send(clientSocket, reinterpret_cast<const char*>(requestResult.response.data()), requestResult.response.size(), 0);
	}
	catch (const std::exception&)
	{
		ErrorResponse error;
		error.message = "An error has occured";
		Buffer msg = JsonResponsePacketSerializer::serializeResponse(error);
		send(clientSocket, reinterpret_cast<const char*>(msg.data()), msg.size(), 0);
		std::cout << "Error has occured" << std::endl;
	}
}

SOCKET Communicator::socketInit()
{
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::exception(__FUNCTION__ " - Socket");
	}

	return serverSocket;
}

