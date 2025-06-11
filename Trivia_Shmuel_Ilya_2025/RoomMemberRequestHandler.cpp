#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_room(room), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo request)
{
    m_room.removeUser(m_user);

    LeaveRoomResponse response{ 0 };
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);

    return { buffer, m_handlerFactory.createMenuRequestHandler(m_user) };
}


RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo request)
{
    RoomStatus status = m_roomManager.getRoomState(m_room.getMetadata().id);
    bool hasGameBegun = status == 1 ? true : false;
    GetRoomStateResponse response{ static_cast<unsigned int>(status), hasGameBegun ,m_room.getAllUsers(),0,m_room.getMetadata().timePerQuestion }; //Fixed wrong response initalization
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return { buffer, nullptr };
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == LEAVE_ROOM_REQUEST ||
        requestInfo.id == GET_ROOM_STATE_REQUEST;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case LEAVE_ROOM_REQUEST:
        return leaveRoom(requestInfo);
    case GET_ROOM_STATE_REQUEST:
        return getRoomState(requestInfo);
    default:
        throw std::exception("invalid request id in RoomMemberRequestHandler");
    }
}