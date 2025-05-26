#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestInfo.h"
#include "RequestResult.h"
#include "LoginManager.h"
#include "TriviaException.h"

LoginRequestHandler::LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory)
    : _loginManager(loginManager), _handlerFactory(handlerFactory) {}

LoginRequestHandler::~LoginRequestHandler()
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == LoginReq || requestInfo.id == SignupReq;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo) 
{
    LoginManager* loginManager = &_loginManager; 
    RequestResult reqRes;

    try 
    {
        if (requestInfo.id == LoginReq)
        {
            reqRes = login(requestInfo);
        }
        else if (requestInfo.id == SignupReq)
        {
            reqRes = signup(requestInfo);
        }
        else 
        {
            throw TriviaException("Invalid request type");
        }
    }
    catch (const std::exception& e) 
    {
        ErrorResponse errorResp{ e.what() };
        reqRes.response = JsonResponsePacketSerializer::serializeResponse(errorResp);
    }

    return reqRes;
}

RequestResult LoginRequestHandler::login(const RequestInfo requestInfo)
{
    LoginManager* loginManager = &_loginManager;
    LoginRequest logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
    std::string username = logReq.username;
    std::string password = logReq.password;

    LoginStatus loginStatus = loginManager->login(username, password);
    LoggedUser user{ username };

    LoginResponse logRes{ loginStatus };

    RequestResult reqRes{ JsonResponsePacketSerializer::serializeResponse(logRes)};

    if (loginStatus == 1 || loginStatus == 2)
    {
        reqRes.newHandler = this;
    }
    else
    {
        reqRes.newHandler = _handlerFactory.createMenuRequestHandler(user);
    }
    return reqRes;
}

RequestResult LoginRequestHandler::signup(const RequestInfo requestInfo)
{
    LoginManager* loginManager = &_loginManager;

    SignupRequest signReq = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
    SignUpStatus signUpStatus;
    std::string username = signReq.username;
    std::string password = signReq.password;
    std::string email = signReq.email;


    SignupResponse signRes{ signUpStatus = loginManager->signup(username, password, email) };
    LoggedUser user{ username };

    RequestResult reqRes { JsonResponsePacketSerializer::serializeResponse(signRes) ,
        _handlerFactory.createMenuRequestHandler(user) };

    return reqRes;
}
