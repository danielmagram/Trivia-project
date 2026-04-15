#pragma once
#include <string>
#include <iostream>

class LoggedUser
{
public:
    LoggedUser(const std::string& username);
    std::string getUsername() const;
private:
    std::string m_username;
};