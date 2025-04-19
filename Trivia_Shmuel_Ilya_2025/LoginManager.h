#pragma once

#include <string>
#include <vector>
#include "IDatabase.h"
#include "SqliteDataBase.h"
#include "LoggedUser.h"


struct LoginStatus 
{
    unsigned int status = 0;
};


struct SignUpStatus 
{
    unsigned int status = 0;
};


class LoginManager
{
public:
    LoginManager(IDatabase* db);

    SignUpStatus signup(const std::string& username, const std::string& password, const std::string& email);
    LoginStatus login(const std::string& username, const std::string& password);
    void logout(const std::string& username);

private:
    IDatabase* m_database;
    std::vector<LoggedUser> m_loggedUsers;
};