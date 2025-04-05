#include "Server.h"

void Server::run()
{
	std::thread handleThread(Communicator::startHandleRequests);
	handleThread.detach();

	while (true)
	{

	}
}
