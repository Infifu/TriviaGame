#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "SqliteDataBase.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "RequestResult.h"


typedef std::vector<unsigned char> Buffer; //same name to match the xml


class LoginRequestHandler : public IRequestHandler 
{
public:
    LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory);
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
private:
    RequestResult login(RequestInfo);
    RequestResult signup(RequestInfo);

    LoginManager& _loginManager;
    RequestHandlerFactory& _handlerFactory;

};