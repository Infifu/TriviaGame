#pragma once
#include <string>
#include <iostream>
#include "json.hpp"
#include "UsingBuffer.h"

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

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(Buffer buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);
};

