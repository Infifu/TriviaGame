#include "Room.h"

Room::Room(RoomData data) : m_metadata(data) {}

void Room::addUser(LoggedUser user)
{
	m_users.push_back(user);
}

void Room::removeUser(LoggedUser user)
{
	m_users.erase(std::remove(m_users.begin(), m_users.end(), user), m_users.end());
}

std::vector<std::string> Room::getAllUsers()
{
	std::vector<std::string> names;
	for (auto const& user : m_users)
	{
		names.push_back(user.getUsername());
	}
	return names;
}

RoomData Room::getMetadata()
{
	return m_metadata;
}

void Room::setStatus(RoomStatus newStatus)
{
	m_metadata.status = newStatus;
}