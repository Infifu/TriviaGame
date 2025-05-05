#include "JsonResponsePacketSerializer.h"


enum requestsCodes
{
    REQUEST_LOGIN = 0,
    REQUEST_ERROR = 1,
    REQUEST_SIGNUP = 2
};

//ERROR 1
//LOGIN 0
//SIGNUP 2

/**
 * @brief serialize error response (converting to binary)
 * @param errorResponse - struct that holds the message
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;
    
    std::string message = errorResponse.message; //store the message
    json messageSerialized = { {"message", message} }; //create the json
    unsigned char errorCode = 1; //store the error code (1 byte)

    jsonDump = json::to_cbor(messageSerialized); //serialize the json
    numberInBinary = intToBytesVal(jsonDump.size()); //convert the number to bytes

    //insert the code
    buffer.push_back(errorCode);
    
    //insert the number in binary
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}


/**
 * @brief serialize login response (converting to binary)
 * @param loginResponse - struct that holds the status
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResponse)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;
    unsigned char loginCode;

    int status = loginResponse.status; //store the message
    json statusSerialized = { {"status", status} }; //create the json
    if (status == 0)
    {
        loginCode = 0; // login successfull
    }
    else
    {
        loginCode = 1; //login failed
    }

    jsonDump = json::to_cbor(statusSerialized); //serialize the json
    numberInBinary = intToBytesVal(jsonDump.size()); //convert the number to bytes

    //push the login code (1 byte)
    buffer.push_back(loginCode);

    //push the number buffer
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());

    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

/**
 * @brief serialize signup response (converting to binary)
 * @param signupResponse - struct that holds the status
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResponse)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;
    unsigned char signUpCode;

    int status = signupResponse.status; //store the message
    json statusSerialized = { {"status", status} }; //create the json
    if (status == 0)
    {
        signUpCode = 0; // signUp successfull
    }
    else
    {
        signUpCode = 1; //signUp failed
    }

    jsonDump = json::to_cbor(statusSerialized); //serialize the json
    numberInBinary = intToBytesVal(jsonDump.size()); //length of the json

    //push the signup code (1 byte)
    buffer.push_back(signUpCode);
    //write the into into the buffer by starting after the signup code
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    int status = response.status; //store the message
    json statusSerialized = { {"status", status} }; //create the json

    unsigned char signUpCode = response.status; //here the codes match (for now)

    jsonDump = json::to_cbor(statusSerialized); //serialize the json
    numberInBinary = intToBytesVal(jsonDump.size()); //length of the json

    //push the signup code (1 byte)
    buffer.push_back(signUpCode);
    //write the into into the buffer by starting after the signup code
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
    return Buffer();
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    return Buffer();
}

Buffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    int status = response.status; //store the message
    json statusSerialized = { {"status", status} }; //create the json

    unsigned char signUpCode = response.status; //here the codes match (for now)

    jsonDump = json::to_cbor(statusSerialized); //serialize the json
    numberInBinary = intToBytesVal(jsonDump.size()); //length of the json

    //push the signup code (1 byte)
    buffer.push_back(signUpCode);
    //write the into into the buffer by starting after the signup code
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    int status = response.status; //store the message
    json statusSerialized = { {"status", status} }; //create the json

    unsigned char signUpCode = response.status; //here the codes match (for now)

    jsonDump = json::to_cbor(statusSerialized); //serialize the json
    numberInBinary = intToBytesVal(jsonDump.size()); //length of the json

    //push the signup code (1 byte)
    buffer.push_back(signUpCode);
    //write the into into the buffer by starting after the signup code
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    //now just push the json
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
    return Buffer();
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
    return Buffer();
}

Buffer JsonResponsePacketSerializer::intToBytesVal(int number)
{
    //little endian encoding cuz god want me to suffer or whatever
    Buffer intBytes;
    intBytes.push_back((number) & 0xFF);         
    intBytes.push_back((number >> 8) & 0xFF);
    intBytes.push_back((number >> 16) & 0xFF);
    intBytes.push_back((number >> 24) & 0xFF); 
    return intBytes;
}
