#pragma once

#include <vector>

// circle not good, bad circle
class IRequestHandler;

struct RequestResult 
{
    std::vector<unsigned char> response;
    IRequestHandler* newHandler;
};

