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

<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const Buffer buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);
<<<<<<< Updated upstream
=======
	//static GetPlayersInRoomRequest deserializeGetPlayersRequest(const Buffer& buffer);
	//static JoinRoomRequest deserializeJoinRoomRequest(const Buffer& buffer);
	//static CreateRoomRequest deserializeCreateRoomRequest(const Buffer& buffer);
>>>>>>> Stashed changes
};

