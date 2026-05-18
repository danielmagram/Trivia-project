#pragma once
#include <string>
#include "LoggedUser.h"
#include <vector>

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	// status - is active: 0 - not active(waiting for players), 1 - active
	unsigned int status;

};

class Room
{
public:
	Room(const RoomData& metadata);
	RoomData getMetadata() const;
	RoomData& getMetadata();
	std::vector<LoggedUser> getAllUsers() const;
	void addUser(const LoggedUser& user);
	void removeUser(const std::string& username);	


private:
		RoomData m_metadata;
		std::vector<LoggedUser> m_users;
};
