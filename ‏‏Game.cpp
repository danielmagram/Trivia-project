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
	auto it = m_players.find(user);
	if (it != m_players.end())
	{
		GameData& info = it->second;
		return m_questions[info.currentQuestion];
	}
}

bool Game::submitAnswer(std::string answer, LoggedUser user)
{
	static auto lastCallTime = std::chrono::steady_clock::now();
	auto it = m_players.find(user);
	if (it != m_players.end())
	{
		auto currentCallTime = std::chrono::steady_clock::now();
		auto diff = currentCallTime - lastCallTime;
		auto ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
		unsigned int timePassed = static_cast<unsigned int>(ms_duration.count());
		lastCallTime = currentCallTime;
		GameData& info = it->second;
		info.currentQuestion;
		if (answer == m_questions[info.currentQuestion].getCorrectAnswer())
		{
			info.currentQuestion += 1;
			info.correctAnswerCount += 1;
			return true;
		}
		info.currentQuestion += 1;
		info.totalAnswerCount += 1;
		return false;
	}
	return false;
	
}

void Game::removePlayer()
{

}