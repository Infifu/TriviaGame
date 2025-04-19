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
    RequestResult result;

    try 
    {
        if (requestInfo.id == 0) 
        {
            auto loginReq = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
            LoginResponse loginResp{ 0 }; 
            result.response = JsonResponsePacketSerializer::serializeResponse(loginResp);
            result.newHandler = nullptr; // Someday
        }
        else if (requestInfo.id == 2) 
        {
            auto signupReq = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
            SignupResponse signupResp{ 0 };
            result.response = JsonResponsePacketSerializer::serializeResponse(signupResp);
            result.newHandler = nullptr; // Someday
        }
        else 
        {
            throw std::runtime_error("Invalid request type");
        }
    }
    catch (const std::exception& e) 
    {
        ErrorResponse errorResp{ e.what() };
        result.response = JsonResponsePacketSerializer::serializeResponse(errorResp);
        result.newHandler = nullptr;
    }

    return result;
}

