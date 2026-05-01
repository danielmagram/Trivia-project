#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* database)
    : m_database(database)
{
}

std::vector<std::string> StatisticsManager::getHighScore() const
{
    return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username) const
{
    std::vector<std::string> userStats;

	// Fetch statistics from the database
    int gamesPlayed = m_database->getNumOfPlayerGames(username);
    int totalAnswers = m_database->getNumOfTotalAnswers(username);
    int correctAnswers = m_database->getNumOfCorrectAnswers(username);
    float averageTime = m_database->getPlayerAverageAnswerTime(username);
    float score = m_database->getPlayerScore(username); 

    // Convert everything to string and push to the vector
    userStats.push_back(std::to_string(gamesPlayed));
    userStats.push_back(std::to_string(totalAnswers));
    userStats.push_back(std::to_string(correctAnswers));
    userStats.push_back(std::to_string(averageTime));
    userStats.push_back(std::to_string(score));

    return userStats;
}