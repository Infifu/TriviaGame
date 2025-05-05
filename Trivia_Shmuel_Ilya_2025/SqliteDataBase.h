#pragma once
#include "IDatabase.h"
#include "UsingBuffer.h"
#include "sqlite3.h"
#include "Question.h"
#include "json.hpp"

using DBvector = std::vector<std::map<std::string, std::string>>;
using RowMap = std::map<std::string, std::string>;

class SqliteDataBase : public IDatabase
{
public:

	SqliteDataBase();
	~SqliteDataBase();
	bool open() override;
	bool close() override;

	bool doesUserExist(const std::string username) override ;
	bool doesPasswordMatch(const std::string username,const std::string password) override;
	void addNewUser(const std::string username,const std::string password,const std::string email) override;
	void importTenQuestions();

	std::vector<Question> getQuestions(int amount);
	float getPlayerAverageAnswerTime(std::string);
	int getNumOfCorrectAnswers(std::string);
	int getNumOfTotalAnswers(std::string);
	int getNumOfPlayerGames(std::string);
	int getPlayerScore(std::string);
	std::vector<std::string> getHighScores();

private:
	DBvector selectQuery(const std::string sqlStatement,const std::string argument);
	bool insertQuery(const std::string table,const std::map<std::string, std::string> values);

private:
	sqlite3* m_database;
};

