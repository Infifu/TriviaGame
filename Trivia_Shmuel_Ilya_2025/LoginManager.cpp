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

    for (const LoggedUser& user : m_loggedUsers)
    {
        if (user.getUsername() == username)
        {
            status = LoginAlreadyLoggedIn; // User already logged in
            return status;
        }
    }

    m_loggedUsers.push_back(LoggedUser(username));
    status = LoginSuccess; // Success
    return status;
}


void LoginManager::logout(const std::string& username)
{
    m_loggedUsers.erase(std::remove(m_loggedUsers.begin(), m_loggedUsers.end(), username), m_loggedUsers.end());
    //maybe compare the sizes to check if it indeed deleted but i think its not needed rn
}
