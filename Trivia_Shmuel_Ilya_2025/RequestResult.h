#pragma once
#include "UsingBuffer.h"

class IRequestHandler;

struct RequestResult 
{
    Buffer response;
    IRequestHandler* newHandler;
};

