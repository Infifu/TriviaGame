#include "GameManager.h"

GameManager::GameManager(IDatabase* database) : m_database(database){}

Game& GameManager::createGame(Room room)
{
    std::vector<Question> questions = m_database->getQuestions(room.getMetadata().numOfQuestionsInGame);
    std::vector<std::string> playersRaw = room.getAllUsers();
    std::map<LoggedUser, GameData> players;
    for (const auto& player : playersRaw)
    {
        players.insert({ LoggedUser(player),GameData(questions[0],0,0,0) });
    }

    auto it = m_games.find(room.getMetadata().id);
    if (it != m_games.end())
    {
        return it->second;
    }

    room.setStatus(RoomStatus::FINISHED);
    return m_games.insert({ room.getMetadata().id, Game(questions, players, room.getMetadata().id, m_database) }).first->second;
}

void GameManager::deleteGame(unsigned int gameId)
{
    m_games.erase(gameId); //check if this works
}

Game& GameManager::getGameById(unsigned int gameID)
{
    return m_games.at(gameID);
}
