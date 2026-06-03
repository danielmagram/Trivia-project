#include "Game.h"
#include <chrono>

Game::Game(std::vector<Question> questions, std::vector<LoggedUser> users, int gameId)
	: m_questions(std::move(questions)), m_gameId(gameId) {
	for (const auto& user : users) {
		m_players[user] = GameData();
	}
}

Game::~Game() {
}

int Game::getId() const
{
	return m_gameId;
}
std::map<LoggedUser, GameData, UserCompare> Game::getPlayers() const
{
	return m_players;
}


Question Game::getQuestionForUser(LoggedUser user)
{
    GameData& playerData = m_players[user];
	m_questionStartTimes[user] = std::chrono::steady_clock::now();
    if (playerData.currentQuestion >= m_questions.size())
    {
        
        throw std::runtime_error("Player has finished all questions");
    }
	return m_questions[playerData.currentQuestion];
}

Question Game::getCurrentQuestion(LoggedUser user)
{
    return m_questions[m_players[user].currentQuestion];
}

bool Game::isGameFinished() const
{
        for (const auto& pair : m_players)
        {
            const GameData& playerData = pair.second;

            if (playerData.currentQuestion < m_questions.size())
            {
                return false;
            }
        }

        return true;
    
}

bool Game::submitAnswer(std::string answer, LoggedUser user) 
{
    auto endTime = std::chrono::steady_clock::now();
    auto startTime = m_questionStartTimes[user];
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    GameData& data = m_players[user];

    data.avrageAnswerTime += static_cast<unsigned int>(duration);
    data.totalAnswerCount++;

    if (answer == m_questions[data.currentQuestion].getCorrectAnswer()) {
        data.correctAnswerCount++;
    }

    if (data.currentQuestion == 9) {
        data.avrageAnswerTime /= 10;
    }

    data.currentQuestion++;
    return true;
}

void Game::removePlayer(LoggedUser user)
{
    if (m_players.find(user) == m_players.end()) {
        return;
    }

    GameData& data = m_players[user];

    if (data.currentQuestion > 0) {
        data.avrageAnswerTime /= data.currentQuestion;
    }

    data.currentQuestion = 10;
    m_questionStartTimes.erase(user);
}