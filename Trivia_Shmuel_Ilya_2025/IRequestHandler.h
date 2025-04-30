#pragma once
#include "UsingBuffer.h"
#include "RequestInfo.h"
#include "RequestResult.h"

class IRequestHandler 
{
public:
    virtual ~IRequestHandler() = default;
    virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
    virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};
