#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "SqliteDataBase.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "RequestResult.h"
#include "UsingBuffer.h"

class RequestHandlerFactory;


class LoginRequestHandler : public IRequestHandler 
{
public:
    LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory);
    ~LoginRequestHandler();
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
private:
    RequestResult login(RequestInfo);
    RequestResult signup(RequestInfo);

    LoginManager& _loginManager;
    RequestHandlerFactory& _handlerFactory;

};