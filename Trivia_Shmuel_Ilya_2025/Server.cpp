#include "Server.h"
#include <string>

Server::Server()
    : m_database(new SqliteDataBase()),
    m_handlerFactory(reinterpret_cast<SqliteDataBase*>(m_database)),
    m_communicator(m_database, &m_handlerFactory)
{
    m_database->open();
}

Server::~Server()
{
    delete m_database;
}

void Server::run()
{
    //open the database
    std::thread handleThread(&Communicator::startHandleRequests, &m_communicator);

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
}
