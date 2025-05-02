#include "RoomManager.h"

void RoomManager::createRoom(LoggedUser user, RoomData roomData)
{
    Room room(roomData);
    m_rooms.insert({ roomData.id,room });
}

void RoomManager::deleteRoom(RoomID id)
{
    m_rooms.erase(id);
}

RoomStatus RoomManager::getRoomState(RoomID id)
{
    return m_rooms.find(id)->second.getMetadata().status;
}

std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> datas;
    
    for (auto& roomPair : m_rooms)
    {
        datas.push_back(roomPair.second.getMetadata());
    }

    return datas;
}

std::optional<Room&> RoomManager::getRoom(RoomID id)
{
    auto it = m_rooms.find(id);
    if (it != m_rooms.end())
    {
        return std::optional<Room&>(it->second);
    }
    else
    {
        return std::nullopt;
    }
}

