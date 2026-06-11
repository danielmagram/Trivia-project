#pragma once
#include "sqlite3.h"
#include <string>
#include "IDatabase.h"
#include <iostream>


class SqliteDatabase : public IDatabase
{
public:
    SqliteDatabase();
    virtual ~SqliteDatabase();
    virtual bool open() override;
    virtual bool close() override;
    virtual bool doesUserExist(const std::string& username) override;
    virtual bool doesPasswordMatch(const std::string& username, const std::string& password) override;
	virtual bool addNewUser(const std::string& username, const std::string& password, const std::string& email, const std::string& address, const std::string& phone, const std::string& dateOfBirth) override;
	virtual void initUserStatistics(const std::string& username) override;
    virtual std::list<Question> getQuestions(int count) override;
	virtual float getPlayerAverageAnswerTime(const std::string& username) override;
	virtual int getNumOfCorrectAnswers(const std::string& username) override;
	virtual int getNumOfTotalAnswers(const std::string& username) override;
	virtual int getNumOfPlayerGames(const std::string& username) override;
	virtual float getPlayerScore(const std::string& username) override;
	virtual std::vector<std::string> getHighScores() override;
    virtual bool submitGameStatistics(std::string username, GameData stats) override;
private:
    sqlite3* m_db;
    sqlite3_stmt* prepareStatement(const std::string& sql) const;
};