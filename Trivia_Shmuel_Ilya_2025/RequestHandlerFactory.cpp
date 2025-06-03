#include "RequestHandlerFactory.h"


RequestHandlerFactory::RequestHandlerFactory(IDatabase* database): _database(database), _roomManager(), _statisticsManager(database), _loginManager(database)
{

}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	LoginRequestHandler* loginRequesthandler = new LoginRequestHandler(_loginManager, *this);
	return loginRequesthandler;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
	MenuRequestHandler* menuRequestHandler = new MenuRequestHandler(user, this, &_roomManager);
	return menuRequestHandler;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return _loginManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
	return new MenuRequestHandler(user, this, &_roomManager);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room room)
{
	return new RoomAdminRequestHandler(user, room, _roomManager, *this);
}

LoginManager& RequestHandlerFactory::getLoginManager() { return _loginManager; }
RoomManager& RequestHandlerFactory::getRoomManager() { return _roomManager; }
StatisticsManager& RequestHandlerFactory::getStatisticsManager() { return _statisticsManager; }