#pragma once
#include <string>
#include <vector>

enum RequestCode
{
    CLOSE_ROOM_REQUEST = 27,
    START_GAME_REQUEST = 28,
    GET_ROOM_STATE_REQUEST = 29,
	LEAVE_ROOM_REQUEST = 30
};

struct StartGameResponse
{
	unsigned int status;
};

struct LeaveRoomResponse
{
	unsigned int status;
};

struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int answerCount;
	unsigned int answerTimeout;
};