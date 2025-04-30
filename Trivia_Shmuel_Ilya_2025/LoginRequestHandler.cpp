#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestInfo.h"
#include "RequestResult.h"
#include "LoginManager.h"

LoginRequestHandler::LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory)
    : _loginManager(loginManager), _handlerFactory(handlerFactory) {}

LoginRequestHandler::~LoginRequestHandler()
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == LoginRec || requestInfo.id == SignupRec;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo) 
{
    LoginManager* loginManager = &_loginManager; 
    RequestResult reqRes;

    try 
    {
        if (requestInfo.id == LoginRec)
        {
            reqRes = login(requestInfo);
        }
        else if (requestInfo.id == SignupRec)
        {
            reqRes = signup(requestInfo);
        }
        else 
        {
            throw std::runtime_error("Invalid request type");
        }
    }
    catch (const std::exception& e) 
    {
        ErrorResponse errorResp{ e.what() };
        reqRes.response = JsonResponsePacketSerializer::serializeResponse(errorResp);
        reqRes.newHandler = nullptr;
    }

    return reqRes;
}

RequestResult LoginRequestHandler::login(const RequestInfo requestInfo)
{
    LoginManager* loginManager = &_loginManager;
    RequestResult reqRes;
    LoginRequest logReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
    LoginResponse logRes;
    std::string username = logReq.username;
    std::string password = logReq.password;

    LoginStatus loginStatus = loginManager->login(username, password);

    logRes.status = loginStatus;
    reqRes.response = JsonResponsePacketSerializer::serializeResponse(logRes);
    reqRes.newHandler = nullptr;
    return reqRes;
}

RequestResult LoginRequestHandler::signup(const RequestInfo requestInfo)
{
    LoginManager* loginManager = &_loginManager;
    RequestResult reqRes;

    SignupRequest signReq = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
    SignUpStatus signUpStatus;
    SignupResponse signRes;
    std::string username = signReq.username;
    std::string password = signReq.password;
    std::string email = signReq.email;


    signUpStatus = loginManager->signup(username, password, email);
    signRes.status = signUpStatus;
    reqRes.response = JsonResponsePacketSerializer::serializeResponse(signRes);
    reqRes.newHandler = nullptr;

    return reqRes;
}
