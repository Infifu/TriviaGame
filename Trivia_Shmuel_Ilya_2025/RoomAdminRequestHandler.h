#pragma once

#include "IRequestHandler.h"
#include "RoomManager.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

enum RequestCode
{
    CLOSE_ROOM_REQUEST = 27,
    START_GAME_REQUEST = 28,
    GET_ROOM_STATE_REQUEST = 29
};

struct LeaveRoomResponse
{
    unsigned int status; 
};

struct StartGameResponse
{
    unsigned int status; 
};

struct GetRoomStateResponse
{
    unsigned int status; 
    unsigned int roomStatus;
};



class RoomAdminRequestHandler : public IRequestHandler
{
private:
    Room m_room;
    LoggedUser m_user;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult closeRoom(RequestInfo request);
    RequestResult startGame(RequestInfo request);
    RequestResult getRoomState(RequestInfo request);

public:
    RoomAdminRequestHandler(LoggedUser user, Room room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
    void setStatus(RoomStatus newStatus);
};