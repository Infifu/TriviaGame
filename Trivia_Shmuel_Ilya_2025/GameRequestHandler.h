#pragma once

#include "IRequestHandler.h"
#include "RoomManager.h"
#include "GameManager.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "Response.h"

class GameRequestHandler : public IRequestHandler
{
public:
    GameRequestHandler(LoggedUser user, Game& game, GameManager& gameManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    LoggedUser m_user;
    Game& m_game;
    GameManager& m_gameManager;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult getQuestion(const RequestInfo& requestInfo);
    RequestResult submitAnswer(const RequestInfo& requestInfo);
    RequestResult getGameResults(const RequestInfo& requestInfo);
    RequestResult leaveGame(const RequestInfo& requestInfo);
};
