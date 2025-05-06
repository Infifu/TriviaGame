#pragma once
#include "UsingBuffer.h"
#include <string>
#include "json.hpp"

using json = nlohmann::json; //so the class could be accessed just by json::foo keyword

<<<<<<< Updated upstream

struct LoginResponse
=======
struct LogoutResponse
>>>>>>> Stashed changes
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct ErrorResponse
{
	std::string message;
};

struct LoginResponse
{
	unsigned int status;
};


class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(const ErrorResponse errorResponse);
	static Buffer serializeResponse(const LoginResponse loginResponse);
	static Buffer serializeResponse(const SignupResponse signupResponse);

<<<<<<< Updated upstream
=======
	//game
	//static Buffer serializeResponse(const LogoutResponse& response);
	//static Buffer serializeResponse(const GetRoomsResponse& response);
	//static Buffer serializeResponse(const GetPlayersInRoomResponse& response);
	//static Buffer serializeResponse(const JoinRoomResponse& response);
	//static Buffer serializeResponse(const CreateRoomResponse& response);
	//static Buffer serializeResponse(const GetHighScoreResponse& response);
	//static Buffer serializeResponse(const GetPersonalStatsResponse& response);
	
>>>>>>> Stashed changes
private:
	static void intToBytesRef(Buffer& buffer,int number);
	static Buffer intToBytesVal(Buffer buffer,int number);



};

