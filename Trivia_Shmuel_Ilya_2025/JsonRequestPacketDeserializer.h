#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;
typedef std::vector<unsigned char> Buffer;

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

