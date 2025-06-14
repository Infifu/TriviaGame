#pragma once
#include "IDatabase.h"
#include "Game.h"
#include "Room.h"


class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
public:
	Game createGame(Room room);
	void deleteGame(unsigned int gameId);
};

