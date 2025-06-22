#pragma once
#include "Communicator.h"
#include "SqliteDatabase.h"
#include "IDatabase.h"

class Server {
public:
	Server();
	~Server();
	void run();
private:
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;
	Communicator m_communicator;

};