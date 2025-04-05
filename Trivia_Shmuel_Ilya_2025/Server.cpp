#include "Server.h"

void Server::run()
{
    std::thread handleThread(&Communicator::startHandleRequests, &m_communicator);
    handleThread.detach();

    while (true)
    {
        // Server main loop
    }
}