#include "Room.h"

Room::Room(const RoomData& metadata) : m_metadata(metadata)
{
}

std::vector<LoggedUser> Room::getAllUsers() const
{
	return m_users;
}

void Room::addUser(const LoggedUser& user)
{
	m_users.push_back(user);

}

void Room::removeUser(const std::string& username)
{
	m_users.erase(std::remove_if(m_users.begin(), m_users.end(),
		[&username](const LoggedUser& user) { return user.getUsername() == username; }),
		m_users.end());
}

RoomData Room::getMetadata() const
{
	return m_metadata;
}

RoomData& Room::getMetadata()
{
	return m_metadata;
}
