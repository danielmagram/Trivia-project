#pragma once
#include<stdio.h>
#include "sqlite3.h"
#include <io.h>
#include <string>

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool doesUserExist(std::string username) = 0;
    virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
    virtual bool addNewUser(std::string username, std::string password, std::string email) = 0;
};