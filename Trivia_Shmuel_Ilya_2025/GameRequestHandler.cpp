#include "GameRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "Response.h"
#include <ctime>

GameRequestHandler::GameRequestHandler(LoggedUser user, Game& game, GameManager& gameManager, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_game(game), m_gameManager(gameManager), m_handlerFactory(handlerFactory)
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == LEAVE_GAME_REQUEST ||
        requestInfo.id == GET_QUESTION_REQUEST ||
        requestInfo.id == SUBMIT_ANSWER_REQUEST ||
        requestInfo.id == GET_GAME_RESULT_REQUEST;
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case LEAVE_GAME_REQUEST:
        return leaveGame(requestInfo);
    case GET_QUESTION_REQUEST:
        return getQuestion(requestInfo);
    case SUBMIT_ANSWER_REQUEST:
        return submitAnswer(requestInfo);
    case GET_GAME_RESULT_REQUEST:
        return getGameResults(requestInfo);
    default:
        throw std::exception("invalid request id in GameRequestHandler");
    }
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo& requestInfo)
{
    Question question = m_game.getQuestionForUser(m_user);

    GetQuestionResponse response{};
    response.status = 0;
    response.question = question.getQuestion();

    std::vector<std::string> possibleAnswers = question.getPossibleAnswers();
    for (unsigned int i = 0; i < possibleAnswers.size(); ++i)
    {
        response.answers[i] = possibleAnswers[i];
    }

    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return { buffer, nullptr };
}


RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo)
{
    SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.buffer);

    double seconds = request.answerTime;

    SubmitAnswerResponse response{};
    response.status = 0;
    response.correctAnswerID = m_game.getQuestionForUser(m_user).getCorrectAnswerId();

    m_game.submitAnswer(m_user, request.answerId, seconds);

    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);

    return { buffer, nullptr };
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo& requestInfo)
{
    GetGameResultsResponse response{};
    response.status = m_game.isGameEnded() == true ? 0 : 1;

    std::vector<PlayerResults> resultsVector;
    std::map<LoggedUser, GameData> players = m_game.getPlayers();
    
    for (auto const& player : players)
    {
        m_game.submitGameStatsToDB(player.second, player.first);
        PlayerResults res{player.first.getUsername(),player.second.correctAnswerCount,player.second.wrongAnswerCount,player.second.averageAnswerTime};
        resultsVector.push_back(res);
    }
     
    response.results = resultsVector;

    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    
    if (m_game.isGameEnded())
    {
        return { buffer, m_handlerFactory.createMenuRequestHandler(m_user)};
    }
    return { buffer, nullptr };
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo& requestInfo)
{
    m_game.removePlayer(m_user);
    LeaveGameResponse response{ 0 };
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return { buffer, m_handlerFactory.createMenuRequestHandler(m_user) };
}
