#include "RequestHandlerFactory.h"


RequestHandlerFactory::RequestHandlerFactory(IDatabase* database): _database(database), _roomManager(), _statisticsManager(database), _loginManager(database),_gameManager(database)
{}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	LoginRequestHandler* loginRequesthandler = new LoginRequestHandler(_loginManager, *this);
	return loginRequesthandler;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
	MenuRequestHandler* menuRequestHandler = new MenuRequestHandler(user, this, &_roomManager,_database);
	return menuRequestHandler;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return _loginManager;
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room& room)
{
	return new RoomAdminRequestHandler(user, room, _roomManager, *this);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room& room)
{
	return new RoomMemberRequestHandler(user, room, _roomManager, *this);
}
                      

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, Room& room)
{
	Game& game = _gameManager.createGame(room);
	return new GameRequestHandler(user, game, _gameManager, *this);
}


RoomManager& RequestHandlerFactory::getRoomManager() { return _roomManager; }
StatisticsManager& RequestHandlerFactory::getStatisticsManager() { return _statisticsManager; }
GameManager& RequestHandlerFactory::getGameManager() { return _gameManager; }