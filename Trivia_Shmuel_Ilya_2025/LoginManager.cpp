#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* db) : m_database(db) {}

SignUpStatus LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    SignUpStatus status;

    if (m_database->doesUserExist(username)) 
    {
        status = SignUsernameIsTaken; // Username already exists
        return status;
    }

    m_database->addNewUser(username, password, email);
    status = SignSuccess; // Success
    return status;
}

LoginStatus LoginManager::login(const std::string& username, const std::string& password)
{
    LoginStatus status;

    if (!m_database->doesUserExist(username)) 
    {
        status = LoginUserNotExist; // User does not exist
        return status;
    }

    if (!m_database->doesPasswordMatch(username, password)) 
    {
        status = LoginWrongPassword; // Incorrect password
        return status;
    }

    m_loggedUsers.push_back(LoggedUser(username));
    status = LoginSuccess; // Success
    return status;
}


void LoginManager::logout(const std::string& username)
{
    for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); ++it)
    {
        if (it->getUsername() == username)
        {
            m_loggedUsers.erase(it);
            std::cout << "User '" << username << "' logged out successfully." << std::endl;
            return;
        }
    }

    std::cout << "User '" << username << "' is not logged in." << std::endl;
}
