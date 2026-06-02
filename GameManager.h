#pragma once
#include <vector>
#include "Game.h"
#include "Room.h"
#include "IDatabase.h"

class IDatabase;

class GameManager {
private:
    IDatabase* m_database;
    std::vector<Game> m_games;
    int m_nextGameId;
public:
    GameManager(IDatabase* db);
    ~GameManager();

    Game createGame(Room room);
    void deleteGame(int gameId);
    void submitGameStatsToDB(LoggedUser user, GameData data);
    int getGameIdByUsername(const std::string& username);
};