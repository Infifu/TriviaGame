#pragma once
#include "Game.h"
#include "Room.h"


class GameManager
{
private:
	IDatabase* m_database;
	std::map<unsigned int, Game> m_games;
public:
	GameManager(IDatabase* database); 
	Game& createGame(Room room);
	void deleteGame(unsigned int gameId);
	Game& getGameById(unsigned int gameID);
};

