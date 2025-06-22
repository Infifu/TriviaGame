#include "Server.h"
#include "WSAInitializer.h"

int main()
{
	WSAInitializer WSAinit;

	Server server;
	server.run();
}