#pragma once
#include "IDatabase.h"

typedef std::vector<std::map<std::string, std::string>> dbVector;


class SqliteDataBase : public IDatabase
{
	bool doesUserExist(std::string username) override;
	bool doesPasswordMatch(std::string username, std::string password) override;
	void addNewUser(std::string username, std::string password, std::string email) override;

	dbVector selectQuery(std::string column, std::string argument);
	bool insertQuery(std::map<std::string, std::string> values);
};

