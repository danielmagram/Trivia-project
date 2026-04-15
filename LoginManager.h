#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <string>
#include <vector>
#include "SqliteDatabase.h"
#include "LoggedUser.h"

struct SignUpStatus
{
    int status;
};
struct LoginStatus
{
    int status;
};

class LoginManager
{
private:
    IDatabase* m_database;
    std::vector<LoggedUser> m_loggedUsers;

public:
    LoginManager(IDatabase* db);
    ~LoginManager() = default;
    SignUpStatus signup(const std::string& username, const std::string& password, const std::string& email);
    LoginStatus login(const std::string& username, const std::string& password);
    void logout(const std::string& username);
};

#endif 