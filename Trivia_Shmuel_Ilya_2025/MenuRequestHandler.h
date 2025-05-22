#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "UsingBuffer.h"
#include "StatisticsManager.h"
#include "TriviaException.h"


class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
private:
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
	unsigned int m_curID;
	JsonResponsePacketSerializer m_serializer;
	JsonRequestPacketDeserializer m_deserializer;
	RoomManager m_manager;

	RequestResult signout(RequestInfo);
	RequestResult getRooms(RequestInfo);
	RequestResult getPlayersInRoom(RequestInfo);
	RequestResult getPeronsalStats(RequestInfo);
	//RequestResult getHighScore(RequestInfo);
	RequestResult joinRoom(RequestInfo);
	RequestResult createRoom(RequestInfo);
public:
	MenuRequestHandler(LoggedUser user, RequestHandlerFactory* handlerFactory);
	bool isRequestRelevant(const RequestInfo& requestInfo) override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;

};

