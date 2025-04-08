#pragma once
#include <iostream>
#include <string>

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
	LoginRequest deserializeLoginRequest(std::string buffer);
	SignupRequest deserializeSignupRequest(std::string buffer);
};

