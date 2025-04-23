#pragma once

#include <vector>
#include <ctime>

using byte = unsigned char; //same name to match the xml

struct RequestInfo 
{
    byte id;
    time_t receivalTime;
    std::vector<byte> buffer;
};
