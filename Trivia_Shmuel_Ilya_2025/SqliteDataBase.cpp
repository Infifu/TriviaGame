#include "SqliteDataBase.h"

SqliteDataBase::SqliteDataBase()
{
	m_database = nullptr;
}

SqliteDataBase::~SqliteDataBase()
{
	close(); //free the db variable
}

bool SqliteDataBase::open()
{
	//TO DO
	//check if the file exists, create the database
	int res = sqlite3_open("TriviaDB.db", &m_database);

	if (res != SQLITE_OK)
	{
		std::cerr << "Failed to open database: " << sqlite3_errmsg(m_database) << std::endl;
		sqlite3_close(m_database);
		return false;
	}
	return true;
}

bool SqliteDataBase::close()
{
	if (sqlite3_close(m_database) == SQLITE_OK)
	{
		return true;
	}
	return false;
}

bool SqliteDataBase::doesUserExist(const std::string username)
{
	std::string statement = "SELECT * FROM users WHERE username = ?;";
	DBvector selected = SqliteDataBase::selectQuery(statement,username);
	
	return !selected.empty();
}

bool SqliteDataBase::doesPasswordMatch(const std::string username,const std::string password)
{
	std::string statement = "SELECT * FROM users WHERE username = ?;";
	DBvector selected = SqliteDataBase::selectQuery(statement,username); //right now username supposed to be individual for everyone
	for (const auto& row : selected)
	{
		if (row.at("password") == password)
		{
			return true;
		}
	}
	return false;
}


//check this function
void SqliteDataBase::addNewUser(const std::string username ,const std::string password,const std::string email)
{
	ArgMap values;
	values["username"] = username;
	values["password"] = password;
	values["email"] = email;

	bool res = SqliteDataBase::insertQuery("users", values);
}

DBvector SqliteDataBase::selectQuery(const std::string sqlStatement,const std::string argument)
{
	sqlite3_stmt* stmt;
	DBvector selected;

	int res = sqlite3_prepare_v2(m_database, sqlStatement.c_str(), -1, &stmt, nullptr);
	if (res != SQLITE_OK)
	{
		std::cerr << "error: Failed to prepare statement" << std::endl;
		return {};
	}
	sqlite3_bind_text(stmt, 1, argument.c_str(), -1, SQLITE_STATIC);

	while ((res = sqlite3_step(stmt) == SQLITE_ROW))
	{
		ArgMap row;
		int colCount = sqlite3_column_count(stmt);

		for (int i = 0; i < colCount; i++)
		{
			std::string column_name = sqlite3_column_name(stmt, i);
			int colType = sqlite3_column_type(stmt, i);

			if (colType == SQLITE_TEXT)
			{
				row[column_name] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
			}
			else if (colType == SQLITE_INTEGER)
			{
				row[column_name] = sqlite3_column_int(stmt, i);
			}
		}
		selected.push_back(row);
	}

	sqlite3_finalize(stmt);
	return selected;
}

bool SqliteDataBase::insertQuery(const std::string table,const ArgMap values)
{
	std::string sqlStatement;
	sqlite3_stmt* stmt;
	char* errMessage = nullptr;

	if (table == "users")
		sqlStatement = "INSERT INTO users (username, password, email) VALUES (?,?,?)";

	int res = sqlite3_prepare_v2(m_database, sqlStatement.c_str(), -1, &stmt, nullptr);
	if (res != SQLITE_OK)
	{
		std::cerr << "error: " << errMessage << std::endl;
		return false;
	}

	sqlite3_bind_text(stmt, 1, values.at("username").c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, values.at("password").c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, values.at("email").c_str(), -1, SQLITE_STATIC);

	res = sqlite3_step(stmt);
	if (res != SQLITE_DONE)
	{
		return false;
	}

	return true;
}

