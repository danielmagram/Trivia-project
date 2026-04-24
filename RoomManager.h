#pragma once

#include "Room.h"

class RoomManager
{
public:
	RoomManager() = default;
	void createRoom(const LoggedUser& creator, const RoomData& roomData);
	void deleteRoom(unsigned int roomId);
	unsigned int getRoomStatus(unsigned int roomId) const;// waiting to start - 0 or active - 1
	std::vector<RoomData> getRooms() const;

private:
	std::vector<Room> m_rooms;
	// Helper function to find a room by its ID
	Room* getRoomById(unsigned int roomId);
	
};