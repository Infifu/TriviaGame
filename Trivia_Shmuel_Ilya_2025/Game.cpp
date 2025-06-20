#include "Game.h"


Game::Game(std::vector<Question> questions, std::map<LoggedUser, GameData> players, unsigned int gameId, IDatabase* database)
{
    m_questions = questions;
    m_players = players;
    m_gameId = gameId;
    m_database = database;
}

Question& Game::getQuestionForUser(const LoggedUser& user)
{
    std::map<LoggedUser, GameData>::iterator it;
    for (it = this->m_players.begin(); it != this->m_players.end(); ++it)
    {
        if (it->first.getUsername() == user.getUsername())
        {
            if (it->second.currentQuestionID < m_questions.size())
            {
                it->second.currentQuestion = m_questions[it->second.currentQuestionID];
                return it->second.currentQuestion; //might crash
            }
        }
    }
}

void Game::submitAnswer(const LoggedUser& user, const unsigned answerId, const double questionTime)
{
    for (auto it = this->m_players.begin(); it != this->m_players.end(); ++it)
    {
        if (it->first.getUsername() == user.getUsername())
        {
            int questionsAmount = it->second.correctAnswerCount + it->second.wrongAnswerCount;

            if (answerId == it->second.currentQuestion.getCorrectAnswerId())
            {
                it->second.correctAnswerCount++;
                it->second.score += 5;
            }
            else
            {
                it->second.wrongAnswerCount++;
            }

            if (questionsAmount == 0)
            {
                it->second.averageAnswerTime = questionTime;
            }
            else
            {
                double totalTime = it->second.averageAnswerTime * questionsAmount;
                totalTime += questionTime;
                it->second.averageAnswerTime = totalTime / (questionsAmount + 1);
            }

            it->second.currentQuestionID++;
            break;
        }
    }
}




void Game::removePlayer(const LoggedUser& user)
{
    auto it = m_players.find(user);
    if (it != m_players.end())
        m_players.erase(it);
}

void Game::submitGameStatsToDB(GameData data, LoggedUser loggedUser)
{
    std::string username = loggedUser.getUsername();

    int gamesCount = m_database->getNumOfPlayerGames(username);
    int existingCorrect = m_database->getNumOfCorrectAnswers(username);
    int existingTotal = m_database->getNumOfTotalAnswers(username);
    float existingAvgTime = m_database->getPlayerAverageAnswerTime(username);
    int existingScore = m_database->getPlayerScore(username);

    int newCorrect = existingCorrect + data.correctAnswerCount;
    int newTotal = existingTotal + data.correctAnswerCount + data.wrongAnswerCount;
    float newAverageTime = 0;

    if (newTotal > 0)
    {
		newAverageTime = ((existingAvgTime * existingTotal) + (data.averageAnswerTime * (data.correctAnswerCount + data.wrongAnswerCount))) / newTotal; //ultra mega calculation
    }

    int newScore = existingScore + data.score;

    std::map<std::string, std::string> values;
    values["username"] = username;
    values["games_played"] = std::to_string(gamesCount + 1);
    values["total_answers"] = std::to_string(newTotal);
    values["correct_answers"] = std::to_string(newCorrect);
    values["average_time"] = std::to_string(newAverageTime);
    values["score"] = std::to_string(newScore);

    m_database->submitGameStatistics(values);
}


unsigned int Game::getgameId()
{
    return m_gameId;
}

bool Game::isGameEnded()
{
    bool end = true;
    for (auto const& player : m_players)
    {
        if (player.second.currentQuestion.getQuestion() != "")
        {
            end = false;
        }
    }
    return end;
}

std::map<LoggedUser, GameData> Game::getPlayers()
{
    return m_players;
}
