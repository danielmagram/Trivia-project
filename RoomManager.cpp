#include "RoomManager.h"

void RoomManager::createRoom(const LoggedUser& creator, RoomData& roomData)
{
    m_roomCounter++;
    roomData.id = m_roomCounter; 

    Room newRoom(roomData);
    newRoom.addUser(creator);
    m_rooms.push_back(newRoom);
}

void RoomManager::deleteRoom(unsigned int roomId)
{
    auto it = std::remove_if(m_rooms.begin(), m_rooms.end(),
        [roomId](const Room& room) { return room.getMetadata().id == roomId; });
    if (it != m_rooms.end()) {
        m_rooms.erase(it, m_rooms.end());
	}
}

unsigned int RoomManager::getRoomStatus(unsigned int roomId) const
{
    Room* room = const_cast<RoomManager*>(this)->getRoomById(roomId);
    if (room) {
        return room->getMetadata().status;
    }
	throw std::runtime_error("Room not found");
	 
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> roomDataList;
    for (const auto& room : m_rooms) {
        if (room.getMetadata().status == 0) 
            roomDataList.push_back(room.getMetadata());
        
    }
	return roomDataList;
}

Room* RoomManager::getRoomById(unsigned int roomId)
{
    auto it = std::find_if(m_rooms.begin(), m_rooms.end(),
        [roomId](const Room& room) { return room.getMetadata().id == roomId; });
    if (it != m_rooms.end()) {
        return &(*it);
    }
    return nullptr;
}
