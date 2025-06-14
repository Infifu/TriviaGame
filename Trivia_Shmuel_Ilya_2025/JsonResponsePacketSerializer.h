 #pragma once
#include "UsingBuffer.h"
#include <string>
#include "Room.h"
#include "json.hpp"
#include "Response.h"

using json = nlohmann::json; //so the class could be accessed just by json::foo keyword

struct GetGameResultsResponse
{
	unsigned int status;
	std::vector<PlayerResults> results;
};

struct PlayerResults
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

//how to serialize playerResults
void to_json(nlohmann::json& j, const PlayerResults& p)
{
	j = nlohmann::json{
		{"username", p.username},
		{"correctAnswerCount", p.correctAnswerCount},
		{"wrongAnswerCount", p.wrongAnswerCount},
		{"averageAnswerTime", p.averageAnswerTime}
	};
}

struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};

struct GetQuestionResponse
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};

struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
};

struct LoginResponse
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

struct CloseRoomResponse 
{
	unsigned int status;
};


class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(const ErrorResponse& errorResponse);
	static Buffer serializeResponse(const LoginResponse& loginResponse);
	static Buffer serializeResponse(const SignupResponse& signupResponse);

	//game
	static Buffer serializeResponse(const LogoutResponse& response);
	static Buffer serializeResponse(const GetRoomsResponse& response);
	static Buffer serializeResponse(const GetPlayersInRoomResponse& response);
	static Buffer serializeResponse(const JoinRoomResponse& response);
	static Buffer serializeResponse(const CreateRoomResponse& response);
	static Buffer serializeResponse(const GetHighScoreResponse& response);
	static Buffer serializeResponse(const GetPersonalStatsResponse& response);

	//room
	static Buffer serializeResponse(const CloseRoomResponse& response);
	static Buffer serializeResponse(const StartGameResponse& response);
	static Buffer serializeResponse(const LeaveRoomResponse& response);
	static Buffer serializeResponse(const GetRoomStateResponse& response);

	//whatever
	static Buffer serializeResponse(const GetGameResultsResponse& response);
	static Buffer serializeResponse(const SubmitAnswerResponse& response);
	static Buffer serializeResponse(const GetQuestionResponse& response);
	static Buffer serializeResponse(const LeaveGameResponse& response);

private:
	static Buffer intToBytesVal(int number);
};

