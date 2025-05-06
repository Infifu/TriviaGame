#pragma once
#include <ctime>
#include "UsingBuffer.h"

struct RequestInfo 
{
    unsigned char id;
    time_t receivalTime;
    Buffer buffer;
};
