#pragma once
#include <string>
#include <list>
#include <vector>
#include "UsingBuffer.h"
#include <iostream>
#include <map>
#include "Question.h"
#include <vector>
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

	virtual std::vector<Question> getQuestions(int) = 0;
	virtual float getPlayerAverageAnswerTime(std::string) = 0;
	virtual int getNumOfCorrectAnswers(std::string) = 0;
	virtual int getNumOfTotalAnswers(std::string) = 0;
	virtual int getNumOfPlayerGames(std::string) = 0;
	virtual int getPlayerScore(std::string) = 0;
	virtual std::vector<std::string> getHighScores() = 0;

	virtual bool submitGameStatistics(std::map<std::string, std::string> values) = 0 ;
	virtual int getPlayersGamesCount(std::string username) = 0;
};

