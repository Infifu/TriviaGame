#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "IDatabase.h"
#include "TriviaException.h"

#define BUFFERSIZE 1024
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
	std::vector<char> charBuffer(BUFFERSIZE);
	LoginRequestHandler* loginRequestHandler = m_handlerFactory.createLoginRequestHandler();
	m_clients.insert(std::pair<SOCKET, LoginRequestHandler*>(clientSocket, loginRequestHandler));

	int sizeRecievesd = recv(clientSocket, &charBuffer[0], BUFFERSIZE, 0);

	Buffer buffer(charBuffer.begin(), charBuffer.end());

	try
	{
		unsigned char code = buffer.at(0);
		RequestInfo requestinfo;
		requestinfo.id = code;
		requestinfo.buffer = buffer;
		requestinfo.receivalTime = std::time(nullptr);


		RequestResult requestResult = loginRequestHandler->handleRequest(requestinfo);

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

