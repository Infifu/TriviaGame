#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "SqliteDataBase.h"

class LoginRequestHandler : public IRequestHandler 
{
public:
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
private:
    std::vector<LoggedUser> _loggedUsers;
    RequestResult login(RequestInfo);
    RequestResult signup(RequestInfo);
    void logOut(LoggedUser user);
};