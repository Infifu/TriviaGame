#pragma once
#include "Room.h"
#include <map>
#include <iostream>
#include <optional>

using RoomID = unsigned int;

class RoomManager
{
public:
	void createRoom(LoggedUser, RoomData);
	void deleteRoom(RoomID id);
	RoomStatus getRoomState(RoomID id);
	std::vector<RoomData> getRooms();
	Room* getRoom(RoomID id); 
private:
	std::map<RoomID, Room> m_rooms;
};

