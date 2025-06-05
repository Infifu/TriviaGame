#pragma once

#include "IRequestHandler.h"
#include "RoomManager.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "Response.h"

class RoomMemberRequestHandler : public IRequestHandler
{
private:
    Room m_room;
    LoggedUser m_user;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult leaveRoom(RequestInfo request);
    RequestResult getRoomState(RequestInfo request);

public:
    RoomMemberRequestHandler(LoggedUser user, Room room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;
};