#include "GameManager.h"
#include "IDatabase.h"
#include <algorithm> 
#define QUESTION_AMOUNT 10

GameManager::GameManager(IDatabase* db) : m_database(db), m_nextGameId(0)
{
}

GameManager::~GameManager()
{
}

Game GameManager::createGame(Room room)
{
    std::list<Question> questionList = m_database->getQuestions(QUESTION_AMOUNT);

    std::vector<Question> questionVector(questionList.begin(), questionList.end());

    m_games.emplace_back(questionVector, room.getAllUsers(), m_nextGameId++);
    return m_games.back();
}

void GameManager::deleteGame(int gameId) {
    auto it = std::find_if(m_games.begin(), m_games.end(), [gameId](const Game& g) {
        return g.getId() == gameId;
        });

    if (it != m_games.end()) {
        std::map<LoggedUser, GameData, UserCompare> players = it->getPlayers();

        for (const auto& pair : players) {
            submitGameStatsToDB(pair.first, pair.second);
        }

        m_games.erase(it);
    }
}

void GameManager::submitGameStatsToDB(LoggedUser user, GameData data)
{
    m_database->submitGameStatistics(user.getUsername(), data);
}