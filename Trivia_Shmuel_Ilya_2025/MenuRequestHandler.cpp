#include "MenuRequestHandler.h"


MenuRequestHandler::MenuRequestHandler(LoggedUser user, RequestHandlerFactory* handlerFactory) : m_user(user), m_handlerFactory(*handlerFactory) , m_curID(0) 
, m_serializer(), m_deserializer()
{}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	if (requestInfo.id > 26 || requestInfo.id < 21)
	{
		return false;
	}
	return true;
}

RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	m_handlerFactory.getLoginManager().logout(m_user.getUsername());
	LogoutResponse logRes{0};
	RequestResult reqRes{m_serializer.serializeResponse(logRes)};
	return reqRes;
}

RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	GetRoomsResponse getRoomRes{ 0, m_manager.getRooms() };
	RequestResult reqRes{ m_serializer.serializeResponse(getRoomRes),nullptr };
	return reqRes;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
	GetPlayersInRoomRequest roomReq = m_deserializer.deserializeGetPlayersRequest(info.buffer);
	GetPlayersInRoomResponse roomRes;
	roomReq.roomId;
	std::optional<Room> roomObject = m_manager.getRoom(roomReq.roomId);
	if (roomObject.has_value())
	{
		roomRes.players = roomObject.value().getAllUsers();
	}
	
	RequestResult reqRes{ m_serializer.serializeResponse(roomRes) , nullptr};
	return reqRes;
}

RequestResult MenuRequestHandler::getPeronsalStats(RequestInfo info)
{
	StatisticsManager stats;
	std::vector<std::string> userStat = stats.getUserStatistics(m_user.getUsername());
	GetPersonalStatsResponse personalStats{ 0, stats.getUserStatistics(m_user.getUsername())};
	RequestResult reqRes{ m_serializer.serializeResponse(personalStats) , nullptr};
	return reqRes;
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
	StatisticsManager stats;
	GetHighScoreResponse highScoreRes{0,stats.getHighScore()};
	RequestResult reqRes{ m_serializer.serializeResponse(highScoreRes),nullptr };
	return reqRes;
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	JoinRoomRequest joinRoomReq = m_deserializer.deserializeJoinRoomRequest(info.buffer);
	JoinRoomResponse joinRoomRes;

	std::optional<Room> optionalRoom = m_manager.getRoom(joinRoomReq.roomId);
	if (optionalRoom.has_value())
	{
		Room room = optionalRoom.value();
		room.addUser(m_user);
		joinRoomRes.status = 0;
	}
	else
	{
		joinRoomRes.status = 1;
	}

	RequestResult reqRes{ m_serializer.serializeResponse(joinRoomRes), nullptr }; //not sure about the new handler

	return reqRes;
}

RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
	CreateRoomRequest createRoomReq = m_deserializer.deserializeCreateRoomRequest(info.buffer);
	RoomData roomData;

	roomData.id = m_curID;
	m_curID++;
	roomData.name = createRoomReq.roomName;
	roomData.maxPlayers = createRoomReq.maxUsers;
	roomData.numOfQuestionsInGame = createRoomReq.questionCount;
	roomData.timePerQuestion = createRoomReq.answerTimeout;
	roomData.status = LOBBY;

	m_manager.createRoom(m_user,roomData);

	CreateRoomResponse roomRes{0};
	
	RequestResult ReqRes{ m_serializer.serializeResponse(roomRes),nullptr};
	return ReqRes;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult reqRes;

	try
	{
		if (!isRequestRelevant(requestInfo))
		{
			throw TriviaException("Request is not relevant to menu");
		}

		switch (requestInfo.id)
		{
		case 3:
			reqRes = signout(requestInfo);
			break;
		case 21:
			reqRes = createRoom(requestInfo);
			break;
		case 22:
			reqRes = joinRoom(requestInfo);
			break;
		case 23:
			reqRes = getRooms(requestInfo);
			break;
		case 24:
			reqRes = getPlayersInRoom(requestInfo);
			break;
		case 25:
			reqRes = getPeronsalStats(requestInfo);
			break;
		case 26:
			reqRes = getHighScore(requestInfo);
			break;
		default:
			throw TriviaException("Error occured in menu handler");
			break;
		}
	}
	catch (const std::exception& e)
	{
		ErrorResponse errRes{e.what()};
		reqRes.response = m_serializer.serializeResponse(errRes);
		reqRes.newHandler = nullptr;
		return reqRes;
	}
	return reqRes;
}