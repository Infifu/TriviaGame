#include "SqliteDataBase.h"

bool SqliteDataBase::open()
{
	//remind me to do something here idkl
	int res = sqlite3_open("Gallery.VC.db", &_database);

	if (res != SQLITE_OK)
	{
		std::cerr << "Failed to open database: " << sqlite3_errmsg(_database) << std::endl;
		sqlite3_close(_database);
		return false;
	}
	return true;
}

bool SqliteDataBase::close()
{
	if (sqlite3_close(_database) == SQLITE_OK)
	{
		return true;
	}
	return false;
}

bool SqliteDataBase::doesUserExist(std::string username)
{
	dbVector selected = SqliteDataBase::selectQuery("username", username);
	if (selected.empty())
		return false;
	else
		return true;
}

bool SqliteDataBase::doesPasswordMatch(std::string username, std::string password)
{
	dbVector selected = SqliteDataBase::selectQuery("username", username);
	for (const auto& row : selected)
	{
		if (row.at("password") == password)
		{
			return true;
		}
	}
	return false;
}

void SqliteDataBase::addNewUser(std::string username , std::string password, std::string email)
{
	std::map<std::string, std::string> values;
	values["username"] = username;
	values["password"] = password;
	values["email"] = email;

	bool res = SqliteDataBase::insertQuery(values);
	//if (!res)
		//throw an error here or whatever
}

std::vector<std::map<std::string, std::string>> SqliteDataBase::selectQuery(std::string column, std::string argument)
{
	sqlite3* db;
	sqlite3_open("TriviaDB.db", &db);


	std::vector<std::map<std::string, std::string>> selected;
	std::string sqlStatement;
	sqlite3_stmt* stmt;

	if (argument.empty())
	{
		sqlStatement = "SELECT * FROM users";
	}
	else
	{
		sqlStatement = "SELECT * FROM users WHERE " + column + " = '" + argument + "';";
	}

	int res = sqlite3_prepare_v2(db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		std::cerr << "error: Failed to prepare statement" << std::endl;
		return {};
	}

	while ((res = sqlite3_step(stmt) == SQLITE_ROW))
	{
		std::map<std::string, std::string> row;
		int colCount = 3;
		for (int i = 0; i < colCount; i++)
		{
			std::string column_name = sqlite3_column_name(stmt, i);
			row[column_name] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
		}
		selected.push_back(row);
	}

	sqlite3_finalize(stmt);
	return selected;
}

bool SqliteDataBase::insertQuery(std::map<std::string, std::string> values)
{
	sqlite3* db;
	sqlite3_open("TriviaDB.db", &db);

	std::string sqlStatement;
	sqlite3_stmt* stmt;
	char* errMessage = nullptr;

	sqlStatement = "INSERT INTO users (username, password, email) VALUES (";
	sqlStatement += "'" + values.at("username") + "', '" + values.at("password") + "', '" + values.at("email") + ");";

	int res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);

	if (res != SQLITE_OK)
	{
		std::cerr << "error: " << errMessage << std::endl;
		return false;
	}
	return true;
}

