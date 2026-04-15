#include "LoginManager.h"

constexpr int DB_FAILED = -1;
constexpr int WRONG_PARAMETERS = 0;
constexpr int SUCCESS = 1;
LoginManager::LoginManager(IDatabase* db)
    : m_database(db)
{
}

SignUpStatus LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    if (username.empty() || password.empty() || email.empty())
    {
        return { WRONG_PARAMETERS };
    }
    try
    {
        if (m_database->doesUserExist(username))
            return { WRONG_PARAMETERS };
        if (m_database->addNewUser(username, password, email))
            return { SUCCESS };
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error: " << e.what() << std::endl;
        return { DB_FAILED };
    }
}
LoginStatus LoginManager::login(const std::string& username, const std::string& password)
{
    for (const auto& user : m_loggedUsers)
    {
        if (user.getUsername() == username)
        {
            return { WRONG_PARAMETERS }; 
        }
    }
    try
    {
        if (!m_database->doesUserExist(username) || !m_database->doesPasswordMatch(username, password))
            return { WRONG_PARAMETERS };

        m_loggedUsers.push_back(LoggedUser(username));

        return { SUCCESS };
    }
    catch (const std::exception& e)
    {
        std::cerr << "DB error: " << e.what() << std::endl;
        return { DB_FAILED };
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