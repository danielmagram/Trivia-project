#include "LoginManager.h"
#include "Constants.h"
#include <algorithm> 
#include <iostream>

LoginManager::LoginManager(IDatabase* db)
    : m_database(db)
{
}

SignUpStatus LoginManager::signup(const std::string& username, const std::string& password, const std::string& email, const std::string& address, const std::string& phone, const std::string& date)
{
    if (username.empty() || password.empty() || email.empty() || address.empty() || phone.empty() || date.empty())
    {
        return { static_cast<unsigned int>(Status::EMPTY_VALUE) };
    }

    try
    {
        if (m_database->doesUserExist(username))
            return { static_cast<unsigned int>(Status::USER_EXISTS) };

        if (m_database->addNewUser(username, password, email, address, phone, date))
        {
            m_database->initUserStatistics(username);
            return { static_cast<unsigned int>(Status::SUCCESS) };
        }

        return { static_cast<unsigned int>(Status::DB_ERROR) };
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error: " << e.what() << std::endl;
        return { static_cast<unsigned int>(Status::DB_ERROR) };
    }
}

LoginStatus LoginManager::login(const std::string& username, const std::string& password)
{
    auto it = std::find_if(m_loggedUsers.begin(), m_loggedUsers.end(),
        [&username](const LoggedUser& user) { return user.getUsername() == username; });

    if (it != m_loggedUsers.end())
    {
        return { static_cast<unsigned int>(Status::USER_ALREADY_LOGGED_IN) };
    }

    try
    {
        if (!m_database->doesUserExist(username) || !m_database->doesPasswordMatch(username, password))
            return { static_cast<unsigned int>(Status::WRONG_PARAMETERS) };

        m_loggedUsers.push_back(LoggedUser(username));

        return { static_cast<unsigned int>(Status::SUCCESS) };
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error: " << e.what() << std::endl;
        return { static_cast<unsigned int>(Status::DB_ERROR) };
    }
}

void LoginManager::logout(const std::string& username)
{
    const std::size_t beforeCount = m_loggedUsers.size();

    m_loggedUsers.erase(std::remove_if(m_loggedUsers.begin(), m_loggedUsers.end(),
        [&username](const LoggedUser& user) { return user.getUsername() == username; }),
        m_loggedUsers.end());

    const std::size_t afterCount = m_loggedUsers.size();
    if (afterCount < beforeCount)
    {
        std::cout << "User logged out: " << username << std::endl;
    }
}