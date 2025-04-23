#pragma once

#include <vector>

//TO DO: verify ts
class IRequestHandler;

struct RequestResult 
{
    std::vector<unsigned char> response;
    IRequestHandler* newHandler;
};

