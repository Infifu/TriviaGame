#include "Question.h"
#include <map>
#include "LoggedUser.h"
#include "IDatabase.h"

struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
	unsigned int currentQuestionID;
};

class Game
{
private:
	IDatabase* m_database;
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	unsigned int m_gameId;
public:
	Game(std::vector<Question> questions, std::map<LoggedUser, GameData> players, unsigned int gameId, IDatabase* database);
	Question& getQuestionForUser(const LoggedUser& user);
	void submitAnswer(const LoggedUser& user, const unsigned answerId, const double questionTime);
	void removePlayer(const LoggedUser& user);
	void submitGameStatsToDB(GameData data, LoggedUser user);
	unsigned int getgameId();
	bool isGameEnded();
	std::map<LoggedUser, GameData> getPlayers();
};

