#include "StatisticsManager.h"

std::vector<std::string> StatisticsManager::getHighScore()
{
    return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
    std::vector<std::string> statistics;
    statistics.push_back("PlayerAverageAnswerTime: " + std::to_string(m_database->getPlayerAverageAnswerTime(username)));
    statistics.push_back("NumOfCorrectAnswers: " + std::to_string(m_database->getNumOfCorrectAnswers(username)));
    statistics.push_back("NumOfTotalAnswers: " + std::to_string(m_database->getNumOfTotalAnswers(username)));
    statistics.push_back("NumOfPlayerGames: " + std::to_string(m_database->getNumOfPlayerGames(username)));
    statistics.push_back("PlayerScore: " + std::to_string(m_database->getPlayerScore(username)));
    return statistics;
}
