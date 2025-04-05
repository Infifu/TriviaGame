#include "Server.h"
#include <string>

void Server::run()
{
	std::thread handleThread(&Communicator::startHandleRequests);
	handleThread.detach();

    std::string command;
    while (true)
    {
        std::getline(std::cin, command);
        if (command == "EXIT")
        {
            break;
            std::cout << "The progrem is Closeing..." << std::endl;
        }
    }
    // TODO: Release resources before exit
}
