#pragma once
#include "IDatabase.h"
#include "UsingBuffer.h"

using DBvector = std::vector<std::map<std::string, std::string>>;
using ArgMap = std::map<std::string, std::string>;

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

private:
	DBvector selectQuery(const std::string sqlStatement,const std::string argument);
	bool insertQuery(const std::string table,const ArgMap values);

private:
	sqlite3* m_database;
};

