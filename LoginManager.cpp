#include "LoginManager.h"
#include "Constants.h"

LoginManager::LoginManager(IDatabase* db)
    : m_database(db)
{
}

SignUpStatus LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    if (username.empty() || password.empty() || email.empty())
    {
        return { static_cast<unsigned int>(Status::EMPTY_VALUE) };
    }

    try
    {
        if (m_database->doesUserExist(username))
            return { static_cast<unsigned int>(Status::USER_EXISTS) };

        if (m_database->addNewUser(username, password, email))
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
    for (const auto& user : m_loggedUsers)
    {
        if (user.getUsername() == username)
        {
            return { static_cast<unsigned int>(Status::USER_ALREADY_LOGGED_IN) };
        }
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
    for (auto user = m_loggedUsers.begin(); user != m_loggedUsers.end(); ++user)
    {
        if (user->getUsername() == username)
        {
            m_loggedUsers.erase(user);
            return;
        }
    }
}