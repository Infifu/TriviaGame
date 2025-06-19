#pragma once
#include "Game.h"
#include "Room.h"


class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
public:
	GameManager(IDatabase* database); 
	Game& createGame(Room room);
	void deleteGame(unsigned int gameId);
};

