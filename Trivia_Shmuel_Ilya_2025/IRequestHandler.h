#pragma once

#include "RequestInfo.h"
#include "RequestResult.h"

class IRequestHandler 
{
public:
    virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
    virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};
