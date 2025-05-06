#pragma once
#include "LoginManager.h"
#include "IDatabase.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h" 

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
private:
	LoginManager _loginManager;
	IDatabase* _database; //IM GOOFY GOOBER RAAAHHHH
public:
	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	LoginManager& getLoginManager();
};

