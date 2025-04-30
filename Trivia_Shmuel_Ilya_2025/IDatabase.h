#pragma once
#include <string>
#include <list>
#include "UsingBuffer.h"
#include <iostream>
#include <map>
#include "sqlite3.h"

class IDatabase
{
public:
	virtual ~IDatabase() {};

	virtual bool open() = 0;
	virtual bool close() = 0;

	virtual bool doesUserExist(const std::string username) = 0;
	virtual bool doesPasswordMatch(const std::string username, const std::string password) = 0;
	virtual void addNewUser(const std::string username, std::string password, const std::string email) = 0;

	//to do:
	//List<Question> getQuestions(int)
	//virtual float getPlayerAverageAnswerTime(std::string);
	//virtual int getNumOfCorrectAnswers(std::string);
	//virtual int getNumOfTotalAnswers(std::string);
	//virtual int getNumOfPlayerGames(std::string);

	//virtual void getSecurityKey(std::string);
};

