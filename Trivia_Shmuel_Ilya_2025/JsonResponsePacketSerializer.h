 #pragma once
#include "UsingBuffer.h"
#include <string>
#include "json.hpp"

using json = nlohmann::json; //so the class could be accessed just by json::foo keyword

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
public:
	static Buffer serializeResponse(const ErrorResponse& errorResponse);
	static Buffer serializeResponse(const LoginResponse& loginResponse);
	static Buffer serializeResponse(const SignupResponse& signupResponse);

private:
	static Buffer intToBytesVal(int number);
};

