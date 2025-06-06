#include "RoomManager.h"

void RoomManager::createRoom(LoggedUser user, RoomData roomData)
{
    roomData.status = LOBBY;
    Room room(roomData);
    room.addUser(user);
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


//Hello dear ido, im rejecting the std::optional<Room> because of the fact that you cant pass it by reference by defult
//only with fuckass workarounds
Room* RoomManager::getRoom(RoomID id)
{
    auto it = m_rooms.find(id);
    if (it != m_rooms.end())
    {
        return &it->second; // copy the Room
    }
    else
    {
        return nullptr;
    }
}

void RoomManager::setRoomStatus(RoomID id, RoomStatus newStatus)
{
    if (m_rooms.find(id) != m_rooms.end())
    {
        m_rooms[id].setStatus(newStatus);
    }
}
