#include "MenuRequestHandler.h"


MenuRequestHandler::MenuRequestHandler(LoggedUser user, RequestHandlerFactory* handlerFactory, RoomManager* roomManger) : m_user(user), m_handlerFactory(*handlerFactory), m_curID(0)
, m_serializer(), m_deserializer(), m_manager(*roomManger)
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
	Room* roomObject = m_manager.getRoom(roomReq.roomId);
	if (roomObject != nullptr)
	{
		roomRes.players = roomObject->getAllUsers();
	}
	
	RequestResult reqRes{ m_serializer.serializeResponse(roomRes) , nullptr};
	return reqRes;
}

RequestResult MenuRequestHandler::getPeronsalStats(RequestInfo info)
{
	IDatabase* db = m_handlerFactory.getDatabase();
	StatisticsManager stats(db);

	JsonRequestPacketDeserializer deserializer;
	GetStatsRequest req = deserializer.deserializeGetStatsRequest(info.buffer);

	std::vector<std::string> userStat = stats.getUserStatistics(req.username);
	GetPersonalStatsResponse personalStats{ 0, userStat };
	return { m_serializer.serializeResponse(personalStats), nullptr };
}


RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
	IDatabase* db = m_handlerFactory.getDatabase();
	StatisticsManager stats(db);

	GetHighScoreResponse highScoreRes{ 0, stats.getHighScore() };
	RequestResult reqRes{ m_serializer.serializeResponse(highScoreRes), nullptr };
	return reqRes;
}



RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
    JoinRoomRequest joinRoomReq = m_deserializer.deserializeJoinRoomRequest(info.buffer);
    JoinRoomResponse joinRoomRes;
	bool found = false;

    Room* room = m_manager.getRoom(joinRoomReq.roomId);
	IRequestHandler* nextHandler = nullptr;

	if (room != nullptr)
    {
		std::vector <std::string> users = room->getAllUsers();
		for (auto user : users)
		{
			if (user == m_user)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			room->addUser(m_user);
			joinRoomRes.status = 0;
			nextHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, *room);
		}
		else
		{
			joinRoomRes.status = 1;
		}
    }
    else
    {
		joinRoomRes.status = 1;
    }

	Buffer buffer = m_serializer.serializeResponse(joinRoomRes);

	return { buffer, nextHandler };
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

	Room* room = m_manager.getRoom(roomData.id);
	IRequestHandler* nextHandler = nullptr;

	if (room != nullptr)
	{
		nextHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, *room);
	}

	CreateRoomResponse roomRes{ 0 };
	Buffer buffer = m_serializer.serializeResponse(roomRes);

	return { buffer, nextHandler };
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