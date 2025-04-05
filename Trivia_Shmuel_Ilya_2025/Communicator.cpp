#include "Communicator.h"

void Communicator::startHandleRequests()
{
	bindAndListen();
}

void Communicator::bindAndListen()
{
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
	{
		throw std::exception(__FUNCTION__ " - Socket");
	}

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
		std::thread thread(&Communicator::handleNewClient, this, client_socket);
		thread.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{

}
