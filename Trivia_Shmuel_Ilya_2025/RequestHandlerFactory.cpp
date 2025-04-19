#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(SqliteDataBase* database) : _database(database), _loginManager(database){}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	LoginRequestHandler* loginRequesthandler = new LoginRequestHandler();
	return loginRequesthandler;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
	return nullptr;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return _loginManager;
} 
