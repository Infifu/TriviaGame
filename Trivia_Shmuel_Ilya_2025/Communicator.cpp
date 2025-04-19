#include "Communicator.h"
#include "RequestHandlerFactory.h"

#define BUFFERSIZE 1024

Communicator::Communicator()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw std::exception("WSAStartup failed");
	}

	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::exception(__FUNCTION__ " - Socket");
	}
}

void Communicator::startHandleRequests()
{
	bindAndListen();
}

void Communicator::bindAndListen()
{

	SOCKET client_socket;
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(6969);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = INADDR_ANY;

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << 6969 << std::endl;

	while (true)
	{
		
		client_socket = accept(m_serverSocket, nullptr, nullptr);

		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "Client accepted. Server and client can speak" << std::endl;
		std::thread thread(&Communicator::handleNewClient, this, std::ref(client_socket));
		thread.detach();
	}
}


//i think i forgot to mutex somewhere but idrc rn so whatever
void Communicator::handleNewClient(SOCKET clientSocket)
{
	std::vector<char> charBuffer(BUFFERSIZE);
	LoginRequestHandler* requestHandler = RequestHandlerFactory::createLoginRequestHandler();
	m_clients.insert(std::pair<SOCKET, LoginRequestHandler*>(clientSocket, requestHandler));

	int sizeRecievesd = recv(clientSocket, &charBuffer[0], BUFFERSIZE, 0);

	Buffer buffer(charBuffer.begin(), charBuffer.end());

	try
	{
		unsigned char code = buffer.at(0);
		RequestResult requestResult;
		RequestInfo requestinfo;
		requestinfo.id = code;
		requestinfo.buffer = buffer;
		requestinfo.receivalTime = std::time(nullptr);


		requestResult = requestHandler->handleRequest(requestinfo);
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

