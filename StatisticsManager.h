#pragma once
#include "IDatabase.h"
#include <vector>
#include <string>

class StatisticsManager
{
public:
    StatisticsManager(IDatabase* database);


    std::vector<std::string> getHighScore() const;

    std::vector<std::string> getUserStatistics(const std::string& username) const;

private:
    IDatabase* m_database;
};