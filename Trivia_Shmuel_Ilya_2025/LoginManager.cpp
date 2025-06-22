#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* db) : m_database(db) {}

SignUpStatus LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    SignUpStatus status;

    std::regex passwordRegex(R"((?=.*[A-Z])(?=.*[a-z])(?=.*\d)(?=.*[\*\&\^\%\$\#\@\!]).{8,})");

    std::regex emailRegex(R"(^[a-zA-Z0-9]+@[a-zA-Z0-9]+(\.com|\.co\.il|\.cyber\.org\.il)$)");

    if (!std::regex_match(password, passwordRegex))
    {
        status = PasswordError;
        return status;
    }

    if (!std::regex_match(email, emailRegex))
    {
        status = EmailError;
        return status;
    }

    if (m_database->doesUserExist(username))
    {
        status = SignUsernameIsTaken;
        return status;
    }

    m_database->addNewUser(username, password, email);
    status = SignSuccess;
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

    for (auto const& user : m_loggedUsers)
    {
        if (user.getUsername() == username)
        {
            status = AlreadyLoggedIn;
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
