#pragma once
#include "IDatabase.h"
#include <vector>


class StatisticsManager
{
private:
	IDatabase* m_database;
public:
	StatisticsManager(IDatabase* database) : m_database(database) {}
	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(std::string username);
};

