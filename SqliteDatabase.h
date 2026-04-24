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
    virtual bool addNewUser(const std::string& username, const std::string& password, const std::string& email) override;
    virtual std::list<Question> getQuestions(int count) override;
private:
    sqlite3* m_db;
};