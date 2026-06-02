#pragma once
#include<stdio.h>
#include "sqlite3.h"
#include <io.h>
#include <string>
#include <list>
#include "GameManager.h"

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool doesUserExist(const std::string& username) = 0;
    virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
    virtual bool addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;
    virtual void initUserStatistics(const std::string& username) = 0;
    virtual std::list<Question> getQuestions(int count) = 0;
    virtual float getPlayerAverageAnswerTime(const std::string& username) = 0; 
    virtual int getNumOfCorrectAnswers(const std::string& username) = 0;
    virtual int getNumOfTotalAnswers(const std::string& username) = 0;
    virtual int getNumOfPlayerGames(const std::string& username) = 0;
    virtual float getPlayerScore(const std::string& username) = 0;
    virtual std::vector<std::string> getHighScores() = 0;
    virtual bool submitGameStatistics(std::string username, GameData stats) = 0;
};