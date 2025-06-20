#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "IDatabase.h"
#include "SqliteDataBase.h"
#include "LoggedUser.h"
#include <regex>

enum LoginStatus
{
    LoginSuccess = 0,
    LoginUserNotExist = 1,
    LoginWrongPassword = 2,
    AlreadyLoggedIn = 3
};

enum SignUpStatus
{
    SignSuccess = 0,
    SignUsernameIsTaken = 1,
    Signfail = 2
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