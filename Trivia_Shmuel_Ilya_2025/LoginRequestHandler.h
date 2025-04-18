#pragma once
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler 
{
public:
    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};