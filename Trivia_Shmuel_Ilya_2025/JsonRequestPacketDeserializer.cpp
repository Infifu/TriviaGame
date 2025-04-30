#include "JsonRequestPacketDeserializer.h"

/**
 * @brief Deserialize a login request from a binary buffer
 * @param buffer - the raw binary data received from the client
 * @return LoginRequest struct containing username and password extracted from json
 */
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer buffer)
{
    LoginRequest request;

    // See if the buffer has at least 5 bytes
    if (buffer.size() < uint32_t(5)) //Just as you wish ido bobido <3
    {
        throw std::runtime_error("Not Valid buffer length For LoginRequest");
    }

    // Extract the 4 sbyte length of the json skiping the data 1 byte
    int length = 0;
    std::memcpy(&length, buffer.data() + 1, sizeof(length));

    Buffer jsonData(buffer.begin() + 5, buffer.begin() + 5 + length); //starting from byte 5 to byte 5 + length

    // Deserialize the json data into a json 
    json j = json::from_cbor(jsonData);

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

    // See that the buffer has at least 5 bytes
    if (buffer.size() < uint32_t(5)) //Just as you wish ido bobido <3
    {
        throw std::runtime_error("Not Valid buffer length For SignupRequest");
    }

    // Extract the 4 sbyte length of the json skiping the data 1 byte
    int length = 0;
    std::memcpy(&length, buffer.data() + 1, sizeof(length));

    // Extract the json data from the buffer starting at byte 5 using the length
    Buffer jsonData(buffer.begin() + 5, buffer.begin() + 5 + length); //starting from byte 5 to byte 5 + length

    // Deserialize the json data into a json 
    json j = json::from_cbor(jsonData);

    request.username = j.at("username").get<std::string>(); // Get the username
    request.password = j.at("password").get<std::string>(); // Get the password
    request.email = j.at("email").get<std::string>();  // Get the email

    return request;
}
