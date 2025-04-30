#pragma once
#include "LoginManager.h"
#include "IDatabase.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h" 

class LoginRequestHandler;


class RequestHandlerFactory
{
private:
	LoginManager _loginManager;
	IDatabase* _database; //need to check ts 🥀🥀🥀💔💔💔
public:
	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();
	LoginManager& getLoginManager();
};

