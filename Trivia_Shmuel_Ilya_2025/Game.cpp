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
    int questionsAmount = 0;

    for (auto it = this->m_players.begin(); it != this->m_players.end(); ++it)
    {
        if (it->first.getUsername() == user.getUsername())
        {
            questionsAmount += it->second.wrongAnswerCount;
            questionsAmount += it->second.correctAnswerCount;

            if (answerId == it->second.currentQuestion.getCorrectAnswerId())
            {
                it->second.correctAnswerCount++;
            }
            else
            {
                it->second.wrongAnswerCount++;
            }

            it->second.averageAnswerTime *= questionsAmount;
            it->second.averageAnswerTime += questionTime;
            it->second.averageAnswerTime /= questionsAmount + 1;

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
    //m_database->submitGameStatistics(data, loggedUser);
}

unsigned int Game::getgameId()
{
    return m_gameId;
}

std::map<LoggedUser, GameData> Game::getPlayers()
{
    return m_players;
}
