#pragma once

#include "Room.h"

class RoomManager
{
public:
	RoomManager() = default;
	void createRoom(const LoggedUser& creator,RoomData& roomData);
	void deleteRoom(unsigned int roomId);
	unsigned int getRoomStatus(unsigned int roomId) const;// waiting to start - 0 or active - 1
	std::vector<RoomData> getRooms() const;
	// Helper function to find a room by its ID (oshri moved it to public)
	Room* getRoomById(unsigned int roomId);

private:
	std::vector<Room> m_rooms;
	unsigned int m_roomCounter = 0;
	
	
};