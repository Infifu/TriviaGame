#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser user, Room& room, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_user(user), m_room(room), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo request)
{
    m_roomManager.setRoomStatus(m_room.getMetadata().id, RoomStatus::FINISHED);
    LeaveRoomResponse response{ 0 };
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return { buffer, m_handlerFactory.createMenuRequestHandler(m_user)};
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo request)
{
    m_roomManager.setRoomStatus(m_room.getMetadata().id, RoomStatus::INGAME);
    StartGameResponse response{ 0 };
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return { buffer, nullptr };
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo request)
{
    RoomStatus status = m_roomManager.getRoomState(m_room.getMetadata().id);
    bool hasGameBegun = status == 1 ? true : false;
    GetRoomStateResponse response{ static_cast<unsigned int>(status), hasGameBegun ,m_room.getAllUsers(),0,m_room.getMetadata().timePerQuestion }; //Fixed wrong response initalization
    Buffer buffer = JsonResponsePacketSerializer::serializeResponse(response);
    return { buffer, nullptr };
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == CLOSE_ROOM_REQUEST ||
        requestInfo.id == START_GAME_REQUEST ||
        requestInfo.id == GET_ROOM_STATE_REQUEST;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    switch (requestInfo.id)
    {
    case CLOSE_ROOM_REQUEST:
        return closeRoom(requestInfo);
    case START_GAME_REQUEST:
        return startGame(requestInfo);
    case GET_ROOM_STATE_REQUEST:
        return getRoomState(requestInfo);
    default:
        throw std::exception("invalid request id in RoomAdminRequestHandler");
    }
}

void RoomAdminRequestHandler::setStatus(RoomStatus newStatus)
{
    m_roomManager.setRoomStatus(m_room.getMetadata().id, newStatus);
}
