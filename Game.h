#pragma once
#include "Question.h"
#include "LoggedUser.h"
#include <map>

struct GameData
{
	unsigned int currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int totalAnswerCount;
	unsigned int avrageAnswerTime;

};

struct UserCompare {
    bool operator()(const LoggedUser& first, const LoggedUser& second) const {
        return first.getUsername() < second.getUsername();
    }
};

class Game {
private:
    std::vector<Question> m_questions;
    std::map<LoggedUser, GameData, UserCompare> m_players;
    int m_gameId;    
public:
    Game(std::vector<Question> questions, std::vector<LoggedUser> users, int gameId);
    ~Game();
    Question getQuestionForUser(LoggedUser user);
    bool submitAnswer(std::string answer, LoggedUser user);
    void removePlayer(LoggedUser user);
    int getId() const;
    std::map<LoggedUser, GameData, UserCompare> getPlayers() const;
};