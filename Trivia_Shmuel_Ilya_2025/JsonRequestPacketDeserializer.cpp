#include "JsonRequestPacketDeserializer.h"

/**
 * @brief Deserialize a login request from a binary buffer
 * @param buffer - the raw binary data received from the client
 * @return LoginRequest struct containing username and password extracted from json
 */
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer buffer)
{
    LoginRequest request;

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

    // Deserialize the json data into a json 
    json j = json::from_cbor(buffer);

    request.username = j.at("username").get<std::string>(); // Get the username
    request.password = j.at("password").get<std::string>(); // Get the password
    request.email = j.at("email").get<std::string>();  // Get the email

    return request;
}