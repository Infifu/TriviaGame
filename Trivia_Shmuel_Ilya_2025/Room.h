#pragma once
#include <iostream>
#include <vector>
#include "LoggedUser.h"

enum RoomStatus
{

};


struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	RoomStatus status;
};


class Room
{
private:
	RoomData m_metadata;
	std::vector <LoggedUser> m_users;
public:
	Room(RoomData data);
	void addUser(LoggedUser);
	void removeUser(LoggedUser);
	std::vector<std::string> getAllUsers();
	RoomData getMetadata();
};

