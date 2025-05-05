#include "JsonRequestPacketDeserializer.h"

/**
 * @brief Deserialize a login request from a binary buffer
 * @param buffer - the raw binary data received from the client
 * @return LoginRequest struct containing username and password extracted from json
 */
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer buffer)
{
    LoginRequest request;

    //// See if the buffer has at least 5 bytes
    //if (buffer.size() < uint32_t(5)) //Just as you wish ido bobido <3
    //{
    //    throw std::runtime_error("Not Valid buffer length For LoginRequest");
    //}

    // Deserialize the json data into a json 
    json j = json::from_cbor(buffer);

    request.username = j.at("username").get<std::string>();  // Get the username
    request.password = j.at("password").get<std::string>();  // Get the password

    return request;
}

/**
 * @brief Deserialize a signup request from a binary buffer
 * @param buffer - the raw binary data received from the client
 * @return SignupRequest struct containing username password and email extracted from json
 */
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
    SignupRequest request;

    //// See that the buffer has at least 5 bytes
    //if (buffer.size() < uint32_t(5)) //Just as you wish ido bobido <3
    //{
    //    throw std::runtime_error("Not Valid buffer length For SignupRequest");
    //}

    // Deserialize the json data into a json 
    json j = json::from_cbor(buffer);

    request.username = j.at("username").get<std::string>(); // Get the username
    request.password = j.at("password").get<std::string>(); // Get the password
    request.email = j.at("email").get<std::string>();  // Get the email

    return request;
}

/**
 * @brief Deserialize a request to get players in a room from a binary buffer
 * @param buffer - the raw binary data received from the client
 * @return GetPlayersInRoomRequest struct with the requested room ID
 */
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const Buffer& buffer) 
{
    GetPlayersInRoomRequest request;
    json j = json::from_cbor(buffer);

    request.roomId = j.at("roomId").get<unsigned int>();

    return request;
}

/**
 * @brief Deserialize a request to join a room from a binary buffer
 * @param buffer - the raw binary data received from the client
 * @return JoinRoomRequest struct with the room ID to join
 */
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer) 
{
    JoinRoomRequest request;
    json j = json::from_cbor(buffer);

    request.roomId = j.at("roomId").get<unsigned int>();

    return request;
}

/**
 * @brief Deserialize a request to create a room from a binary buffer
 * @param buffer - the raw binary data received from the client
 * @return CreateRoomRequest struct with room name max users question count and answer timeout
 */
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer) 
{
    CreateRoomRequest request;
    json j = json::from_cbor(buffer);

    request.roomName = j.at("roomName").get<std::string>();
    request.maxUsers = j.at("maxUsers").get<unsigned int>();
    request.questionCount = j.at("questionCount").get<unsigned int>();
    request.answerTimeout = j.at("answerTimeout").get<unsigned int>();

    return request;
}

