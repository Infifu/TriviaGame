#include "JsonResponsePacketSerializer.h"

//ERROR 1
//LOGIN 0
//SIGNUP 2

/**
 * @brief serialize error response (converting to binary)
 * @param errorResponse - struct that holds the message
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse errorResponse)
{
    Buffer jsonDump;
    Buffer buffer;

    std::string message = errorResponse.message; //store the message
    json messageSerialized = { {"message", message} }; //create the json
    unsigned char errorCode = 1; //store the error code (1 byte)

    jsonDump = json::to_cbor(messageSerialized); //serialize the json
    int jsonLength = jsonDump.size(); //length of the json

    //push the error code (1 byte)
    buffer.push_back(errorCode);
    //resize the buffer to fit the int
    buffer.resize(buffer.size() + sizeof(jsonLength));
    //write the into into the buffer by starting after the erro code
    std::memcpy(buffer.data() + 1, &jsonLength, sizeof(jsonLength));
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}


/**
 * @brief serialize login response (converting to binary)
 * @param loginResponse - struct that holds the status
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse loginResponse)
{
    Buffer jsonDump;
    Buffer buffer;

    int status = loginResponse.status; //store the message
    json statusSerialized = { {"status", status} }; //create the json
    unsigned char loginCode = 0; //store the login code (1 byte)

    jsonDump = json::to_cbor(statusSerialized); //serialize the json
    int jsonLength = jsonDump.size(); //length of the json

    //push the login code (1 byte)
    buffer.push_back(loginCode);
    //resize the buffer to fit the int
    buffer.resize(buffer.size() + sizeof(jsonLength));
    //write the into into the buffer by starting after the login code
    std::memcpy(buffer.data() + 1, &jsonLength, sizeof(jsonLength));
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

/**
 * @brief serialize signup response (converting to binary)
 * @param loginResponse - struct that holds the status
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse signupResponse)
{
    Buffer jsonDump;
    Buffer buffer;

    int status = signupResponse.status; //store the message
    json statusSerialized = { {"status", status} }; //create the json
    unsigned char signUpCode = 2; //store the signup code (1 byte)

    jsonDump = json::to_cbor(statusSerialized); //serialize the json
    int jsonLength = jsonDump.size(); //length of the json

    //push the signup code (1 byte)
    buffer.push_back(signUpCode);
    //resize the buffer to fit the int
    buffer.resize(buffer.size() + sizeof(jsonLength));
    //write the into into the buffer by starting after the signup code
    std::memcpy(buffer.data() + 1, &jsonLength, sizeof(jsonLength));
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}