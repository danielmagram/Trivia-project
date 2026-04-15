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
    virtual bool doesUserExist(const std::string& username) = 0;
    virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;
    virtual bool addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;
};