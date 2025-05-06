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

class JsonRequestPacketDeserializer
{
public:
	//I removed the length and code extraction from youre deserializer please note this
	//and dont make it in new ones
	static LoginRequest deserializeLoginRequest(const Buffer buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);
};

