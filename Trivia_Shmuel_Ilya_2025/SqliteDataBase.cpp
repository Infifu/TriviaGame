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


void SqliteDataBase::addNewUser(const std::string username ,const std::string password,const std::string email)
{
	std::map<std::string, std::string> values;
	values["username"] = username;
	values["password"] = password;
	values["email"] = email;

	bool res = SqliteDataBase::insertQuery("users", values);

	if (res) 
	{
		std::map<std::string, std::string> statsValues;
		statsValues["username"] = username;
		statsValues["games_played"] = "0";
		statsValues["total_answers"] = "0";
		statsValues["correct_answers"] = "0";
		statsValues["average_time"] = "0.0";
		statsValues["score"] = "0";

		SqliteDataBase::insertQuery("statistics", statsValues);
	}
}



void SqliteDataBase::importTenQuestions()
{
	const char* insertSQL = R"(
        INSERT INTO questions (question, answer0, answer1, answer2, answer3, correct_answer_id)
        VALUES (?, ?, ?, ?, ?, ?);
    )";

	std::vector<Question> questions = {
		Question("What is the capital of France?", {"Berlin", "Madrid", "Paris", "Rome"}, 2),
		Question("Which planet is known as the Red Planet?", {"Earth", "Mars", "Jupiter", "Venus"}, 1),
		Question("Who wrote 'Hamlet'?", {"Charles Dickens", "William Shakespeare", "Leo Tolstoy", "Mark Twain"}, 1),
		Question("Which element has the chemical symbol 'O'?", {"Gold", "Oxygen", "Iron", "Silver"}, 1),
		Question("How many continents are there?", {"5", "6", "7", "8"}, 2),
		Question("What is the square root of 64?", {"6", "7", "8", "9"}, 2),
		Question("In which year did World War II end?", {"1942", "1944", "1945", "1946"}, 2),
		Question("Which language is primarily spoken in Brazil?", {"Spanish", "Portuguese", "English", "French"}, 1),
		Question("What is the largest mammal?", {"Elephant", "Blue Whale", "Giraffe", "Hippo"}, 1),
		Question("Who painted the Mona Lisa?", {"Vincent Van Gogh", "Pablo Picasso", "Leonardo da Vinci", "Michelangelo"}, 2)
	};

	sqlite3_stmt* stmt = nullptr;
	for (const auto& q : questions) 
	{
		const auto& answers = q.getPossibleAnswers();

		if (sqlite3_prepare_v2(m_database, insertSQL, -1, &stmt, nullptr) == SQLITE_OK) 
		{
			sqlite3_bind_text(stmt, 1, q.getQuestion().c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 2, answers[0].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 3, answers[1].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 4, answers[2].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(stmt, 5, answers[3].c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 6, q.getCorrectAnswerId());   

			sqlite3_step(stmt);
			sqlite3_reset(stmt);
		}
	}
	sqlite3_finalize(stmt);
}

std::vector<Question> SqliteDataBase::getQuestions(int amount)
{
	std::string sqlstmt = "SELECT * FROM questions ORDER BY RANDOM() LIMIT ?";
	std::vector<Question> questions;
	std::vector<std::string> possibleAnswers;
	DBvector selected = selectQuery(sqlstmt, std::to_string(amount));
	for (auto const& row : selected)
	{
		possibleAnswers.clear();
		possibleAnswers.push_back(row.at("answer0"));
		possibleAnswers.push_back(row.at("answer1"));
		possibleAnswers.push_back(row.at("answer2"));
		possibleAnswers.push_back(row.at("answer3"));
		questions.push_back(Question(row.at("question"),possibleAnswers,std::stoi(row.at("correct_answer_Index"))));
	}
	return questions;
}

float SqliteDataBase::getPlayerAverageAnswerTime(std::string username)
{
	std::string sqlstmt = "SELECT * FROM statistics WHERE username = ?";
	DBvector selected = selectQuery(sqlstmt, username);
	for (auto const& row : selected)
	{
		std::cout << row.at("average_time") << std::endl;
		return std::stof(row.at("average_time"));
	}
}

int SqliteDataBase::getNumOfCorrectAnswers(std::string username)
{
	std::string sqlstmt = "SELECT * FROM statistics WHERE username = ?";
	DBvector selected = selectQuery(sqlstmt, username);
	for (auto const& row : selected)
	{
		return std::stoi(row.at("correct_answers"));
	}
}

int SqliteDataBase::getNumOfTotalAnswers(std::string username)
{
	std::string sqlstmt = "SELECT * FROM statistics WHERE username = ?";
	DBvector selected = selectQuery(sqlstmt, username);
	for (auto const& row : selected)
	{
		return std::stoi(row.at("total_answers"));
	}
}

int SqliteDataBase::getNumOfPlayerGames(std::string username)
{
	std::string sqlstmt = "SELECT * FROM statistics WHERE username = ?";
	DBvector selected = selectQuery(sqlstmt, username);
	for (auto const& row : selected)
	{
		return std::stoi(row.at("games_played"));
	}
}

int SqliteDataBase::getPlayerScore(std::string username)
{
	std::string sqlstmt = "SELECT * FROM statistics WHERE username = ?";
	DBvector selected = selectQuery(sqlstmt, username);
	for (auto const& row : selected)
	{
		return std::stoi(row.at("score"));
	}
}

std::vector<std::string> SqliteDataBase::getHighScores()
{
	std::vector<std::string> highScores;
	std::string sqlstmt = "SELECT username, score FROM statistics ORDER BY score DESC LIMIT 3";
	DBvector selected = selectQuery(sqlstmt, "");

	for (const auto& row : selected)
	{
		std::string name = row.at("username");
		std::string score = row.at("score");
		std::cout << name << " " << score << std::endl;
		highScores.push_back(name + ": " + score);
	}

	return highScores;
}

bool SqliteDataBase::submitGameStatistics(GameData gameData, LoggedUser loggedUser)
{
	int totalAnswer = gameData.correctAnswerCount + gameData.wrongAnswerCount;

	std::map<std::string, std::string> statsValues;
	statsValues["username"] = loggedUser.getUsername();
	statsValues["games_played"] = "0"; //FIX THIS LATER
	statsValues["total_answers"] = std::to_string(totalAnswer);
	statsValues["correct_answers"] = std::to_string(gameData.correctAnswerCount);
	statsValues["average_time"] = std::to_string(gameData.averageAnswerTime);
	statsValues["score"] = "0"; //calculate score //maybe delete this later

	SqliteDataBase::insertQuery("statistics", statsValues);
}


DBvector SqliteDataBase::selectQuery(const std::string sqlStatement, const std::string argument)
{
	DBvector selected;
	sqlite3_stmt* stmt;

	int res = sqlite3_prepare_v2(m_database, sqlStatement.c_str(), -1, &stmt, nullptr);
	if (res != SQLITE_OK)
	{
		std::cerr << "error: Failed to prepare statement" << std::endl;
		return {};
	}
	if (!argument.empty())
	{
		sqlite3_bind_text(stmt, 1, argument.c_str(), -1, SQLITE_STATIC);
	}

	while ((res = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		RowMap row;
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
				row[column_name] = std::to_string(sqlite3_column_int(stmt, i));
			}
			else if (colType == SQLITE_FLOAT)
			{
				row[column_name] = std::to_string(sqlite3_column_double(stmt, i));
			}
			else if (colType == SQLITE_NULL)
			{
				row[column_name] = "";
			}
		}
		selected.push_back(row);
	}

	sqlite3_finalize(stmt);
	return selected;
}



bool SqliteDataBase::insertQuery(const std::string table, const std::map<std::string, std::string> values)
{
	std::string sqlStatement;
	sqlite3_stmt* stmt;
	char* errMessage = nullptr;

	if (table == "users")
		sqlStatement = "INSERT INTO users (username, password, email) VALUES (?,?,?)";
	else if (table == "statistics")
		sqlStatement = "INSERT INTO statistics (username, games_played, total_answers, correct_answers, average_time, score) VALUES (?,?,?,?,?,?)";
	else
	{
		std::cerr << "error: Invalid table name" << std::endl;
		return false;
	}

	int res = sqlite3_prepare_v2(m_database, sqlStatement.c_str(), -1, &stmt, nullptr);
	if (res != SQLITE_OK)
	{
		std::cerr << "prepare error: " << sqlite3_errmsg(m_database) << std::endl;
		return false;
	}

	if (table == "statistics")
	{
		sqlite3_bind_text(stmt, 1, values.at("username").c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(stmt, 2, std::stoi(values.at("games_played")));
		sqlite3_bind_int(stmt, 3, std::stoi(values.at("total_answers")));
		sqlite3_bind_int(stmt, 4, std::stoi(values.at("correct_answers")));
		sqlite3_bind_double(stmt, 5, std::stod(values.at("average_time")));
		sqlite3_bind_int(stmt, 6, std::stoi(values.at("score")));
	}
	else if (table == "users")
	{
		sqlite3_bind_text(stmt, 1, values.at("username").c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, values.at("password").c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, values.at("email").c_str(), -1, SQLITE_STATIC);
	}

	res = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	if (res != SQLITE_DONE)
	{
		std::cerr << "error: " << sqlite3_errmsg(m_database) << std::endl;
		return false;
	}

	return true;
}

bool SqliteDataBase::updateStatistics(std::map<std::string, std::string> values)
{
	std::string sqlStatement;
	sqlite3_stmt* stmt;
	char* errMessage = nullptr;

	sqlStatement = "UPDATE statistics SET games_played = ?, total_answers = ? , correct_answer = ?, average_time = ?, score = ? WHERE username = ?";

	int res = sqlite3_prepare_v2(m_database, sqlStatement.c_str(), -1, &stmt, nullptr);
	if (res != SQLITE_OK)
	{
		std::cerr << "prepare error: " << sqlite3_errmsg(m_database) << std::endl;
		return false;
	}

	sqlite3_bind_int(stmt, 1, std::stoi(values.at("games_played")));
	sqlite3_bind_int(stmt, 2, std::stoi(values.at("total_answers")));
	sqlite3_bind_int(stmt, 3, std::stoi(values.at("correct_answers")));
	sqlite3_bind_double(stmt, 4, std::stod(values.at("average_time")));
	sqlite3_bind_int(stmt, 5, std::stoi(values.at("score")));
	sqlite3_bind_text(stmt, 6, values.at("username").c_str(), -1, SQLITE_STATIC);

	res = sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	if (res != SQLITE_DONE) {
		std::cerr << "SQL error: " << sqlite3_errmsg(m_database) << std::endl;
		return false;
	}

	return true;
}
