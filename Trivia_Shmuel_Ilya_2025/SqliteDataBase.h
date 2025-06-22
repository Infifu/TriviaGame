#pragma once
#include "IDatabase.h"
#include "UsingBuffer.h"
#include "sqlite3.h"
#include "Question.h"
#include "json.hpp"
#include <Vector>
#include "LoggedUser.h"

using DBvector = std::vector<std::map<std::string, std::string>>;
using RowMap = std::map<std::string, std::string>;

class SqliteDataBase : public IDatabase
{
public:

	SqliteDataBase();
	~SqliteDataBase();
	bool open() override;
	bool createTables();
	bool close() override;

	bool doesUserExist(const std::string username) override ;
	bool doesPasswordMatch(const std::string username,const std::string password) override;
	void addNewUser(const std::string username,const std::string password,const std::string email) override;
	void importTenQuestions();

	std::vector<Question> getQuestions(int amount) override;
	float getPlayerAverageAnswerTime(std::string) override;
	int getNumOfCorrectAnswers(std::string) override;
	int getNumOfTotalAnswers(std::string) override;
	int getNumOfPlayerGames(std::string) override;
	int getPlayerScore(std::string) override;
	std::vector<std::string> getHighScores() override;

	bool submitGameStatistics(std::map<std::string, std::string> values) override;
	int getPlayersGamesCount(std::string username) override;

	bool uploadQuestion(std::map<std::string, std::string> values);

private:
	DBvector selectQuery(const std::string sqlStatement,const std::string argument);
	bool insertQuery(const std::string table,const std::map<std::string, std::string> values);
	bool updateQuery(const std::map<std::string, std::string> values);
private:
	sqlite3* m_database;
};

