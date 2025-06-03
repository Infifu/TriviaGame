#pragma once
#include "LoginManager.h"
#include "IDatabase.h"
#include "MenuRequestHandler.h"
#include "LoginRequestHandler.h" 
#include "RoomManager.h"
#include "RoomAdminRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
private:
	LoginManager _loginManager;
	IDatabase* _database; //IM GOOFY GOOBER RAAAHHHH
	RoomManager _roomManager;
	StatisticsManager _statisticsManager;
public:
	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	LoginManager& getLoginManager();
	IDatabase* getDatabase() const { return _database; }

	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, Room room);
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();

};

