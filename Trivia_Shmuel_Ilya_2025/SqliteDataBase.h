#pragma once
#include "IDatabase.h"
#include "UsingBuffer.h"

using DBvector = std::vector<std::map<std::string, std::string>>;


class SqliteDataBase : public IDatabase
{
public:

	SqliteDataBase();
	~SqliteDataBase();
	bool open() override;
	bool close() override;

	bool doesUserExist(std::string username) override ;
	bool doesPasswordMatch(std::string username, std::string password) override;
	void addNewUser(std::string username, std::string password, std::string email) override;

private:
	DBvector selectQuery(std::string sqlStatement,std::string argument);
	bool insertQuery(std::string table, std::map<std::string, std::string> values);

private:
	sqlite3* m_database;
};

