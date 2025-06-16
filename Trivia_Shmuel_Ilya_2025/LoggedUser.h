#pragma once

#include <string>

class LoggedUser 
{
public:
    LoggedUser(const std::string& username) : m_username(username) {}
    std::string getUsername() const;

    //TO DO:
    //need this for log out might delete later sir
    bool operator==(const LoggedUser& other) const { return getUsername() == other.getUsername(); }
    bool operator<(const LoggedUser& other) const { return getUsername() < other.getUsername(); }
<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
private:

    std::string m_username;
};
