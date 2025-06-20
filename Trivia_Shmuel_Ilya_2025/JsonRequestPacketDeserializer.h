#pragma once
#include <string>
#include <iostream>
#include "json.hpp"
#include "UsingBuffer.h"
#include <cstdint> //for the uint32_t

using json = nlohmann::json;

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

struct GetPlayersInRoomRequest 
{
	unsigned int roomId;
};

struct JoinRoomRequest 
{
	unsigned int roomId;
};

struct CreateRoomRequest 
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct GetStatsRequest
{
	std::string username;
};

struct CloseRoomRequest {};
struct StartGameRequest {};
struct GetRoomStateRequest {};
struct LeaveRoomRequest {};

struct SubmitAnswerRequest
{
	unsigned int answerId;
	unsigned int answerTime;

};


class JsonRequestPacketDeserializer
{
public:
	//I removed the length and code extraction from your deserializer please note this
	//and dont make it in new ones
	static GetStatsRequest deserializeGetStatsRequest(Buffer buffer);
	static LoginRequest deserializeLoginRequest(const Buffer buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const Buffer& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer& buffer);

	static CloseRoomRequest deserializeCloseRoomRequest(Buffer);
	static StartGameRequest deserializeStartGameRequest(Buffer);
	static GetRoomStateRequest deserializeGetRoomStateRequest(Buffer);
	static LeaveRoomRequest deserializeLeaveRoomRequest(Buffer);

	static SubmitAnswerRequest deserializeSubmitAnswerRequest(Buffer buffer);

};

