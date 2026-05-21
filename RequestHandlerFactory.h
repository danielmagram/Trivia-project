#pragma once
#include "StatisticsManager.h" 
#include "RoomManager.h" 
#include "LoginManager.h" 
#include "IDatabase.h"   
#include "IRequestHandler.h" 
#include <memory>
// Forward declarations to avoid circular dependencies
class Room;
class LoggedUser;

class RequestHandlerFactory
{
public:
    RequestHandlerFactory();
    ~RequestHandlerFactory();

    // The return types must be the Base Interface!
    std::unique_ptr<IRequestHandler> createLoginRequestHandler();
    std::unique_ptr<IRequestHandler> createMenuRequestHandler(const std::string& user);
    std::unique_ptr<IRequestHandler> createRoomAdminRequestHandler(LoggedUser loggedUser, Room* room);
    std::unique_ptr<IRequestHandler> createRoomMemberRequestHandler(LoggedUser loggedUser, Room* room);

    LoginManager& getLoginManager();
    RoomManager& getRoomManager();
    StatisticsManager& getStatisticsManager();
private:
    IDatabase* m_database;
    LoginManager m_loginManager;
    RoomManager m_roomManager;
    StatisticsManager m_statisticsManager;
};