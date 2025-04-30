#include "RequestHandlerFactory.h"


RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) : _loginManager(database), _database(database) {}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	LoginRequestHandler* loginRequesthandler = new LoginRequestHandler(_loginManager, *this);
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
