#pragma once

#include <string>

class LoggedUser 
{
public:
    LoggedUser(const std::string& username) : m_username(username) {}
    std::string getUsername() const { return m_username; }

private:
    std::string m_username;
};
