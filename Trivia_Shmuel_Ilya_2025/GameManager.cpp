#include "GameManager.h"

Game GameManager::createGame(Room room)
{
    std::vector<Question> questions = m_database->getQuestions(room.getMetadata().numOfQuestionsInGame);
    std::vector<std::string> playersRaw = room.getAllUsers();
    std::map<LoggedUser, GameData> players;
    for (const auto& player : playersRaw)
    {
        players.insert({ LoggedUser(player),GameData(questions[0],0,0,0) });
    }
    Game game(questions, players, room.getMetadata().id, m_database);
    return game;
}

void GameManager::deleteGame(unsigned int gameId)
{
    for (auto it = m_games.begin(); it != m_games.end(); )
    {
        if (it->getgameId() == gameId)
        {
            it = m_games.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
}
