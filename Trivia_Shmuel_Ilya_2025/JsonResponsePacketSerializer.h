#pragma once
#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json; //so the class could be accessed just by json::foo keyword
typedef std::vector<unsigned char> Buffer; //same name to match the xml

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

class JsonResponsePacketSerializer
{
	static Buffer serializeResponse(ErrorResponse errorResponse);
	static Buffer serializeResponse(LoginResponse loginResponse);
	static Buffer serializeResponse(SignupResponse signupResponse);
};

