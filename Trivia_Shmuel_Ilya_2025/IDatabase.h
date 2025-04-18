#pragma once
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <map>
#include "sqlite3.h"

class IDatabase
{
	virtual bool doesUserExist(std::string username) = 0;
	virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
	virtual void addNewUser(std::string username, std::string password, std::string email) = 0;

	//to do:
	//List<Question> getQuestions(int)
	//virtual float getPlayerAverageAnswerTime(std::string);
	//virtual int getNumOfCorrectAnswers(std::string);
	//virtual int getNumOfTotalAnswers(std::string);
	//virtual int getNumOfPlayerGames(std::string);

	//virtual void getSecurityKey(std::string);
};

