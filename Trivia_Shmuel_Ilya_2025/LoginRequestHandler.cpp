#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestInfo.h"
#include "RequestResult.h"

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == 0 || requestInfo.id == 2;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo) 
{
    RequestResult reqRes;

    try 
    {
        if (requestInfo.id == 0) 
        {
            reqRes = login(requestInfo);
        }
        else if (requestInfo.id == 2) 
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

RequestResult LoginRequestHandler::login(RequestInfo requestInfo)
{
    LoginRequest loginReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
    LoginResponse loginRes;
    RequestResult reqRes;

    //TO DO: check if this could be done in a diffrent way
    SqliteDataBase database;
    if (database.doesUserExist(loginReq.username))
    {
        if (database.doesPasswordMatch(loginReq.username, loginReq.password))
        {
            _loggedUsers.push_back(LoggedUser(loginReq.username));
            loginRes.status = 0;
        }
        else
        {
            loginRes.status = 2; //password doesnt match
        }
    }
    else
    {
        loginRes.status = 1; //doesnt exist
    }

    reqRes.response = JsonResponsePacketSerializer::serializeResponse(loginRes);
    reqRes.newHandler = nullptr; //TO DO: complete

    return reqRes;
}

RequestResult LoginRequestHandler::signup(RequestInfo requestInfo)
{
    SignupRequest signUpReq = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
    SignupResponse signUpRes;
    RequestResult reqRes;

    //TO DO: check if this could be done in a diffrent way
    //Check if username 
    SqliteDataBase database;

    if (!database.doesUserExist(signUpReq.username)) //do the same with mail
    {
        database.addNewUser(signUpReq.username, signUpReq.password, signUpReq.email);
        signUpRes.status = 0;
    }
    else
    {
        signUpRes.status = 1; // username taken
    }
    
    reqRes.response = JsonResponsePacketSerializer::serializeResponse(signUpRes);
    reqRes.newHandler = nullptr; //TO DO: complete

    return reqRes;
}

//TO DO: verify this
void LoginRequestHandler::logOut(LoggedUser user)
{
    //the so called erase - remove idiom
    _loggedUsers.erase(std::remove(_loggedUsers.begin(), _loggedUsers.end(), user),_loggedUsers.end());
}

