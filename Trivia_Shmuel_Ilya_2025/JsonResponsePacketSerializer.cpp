#include "JsonResponsePacketSerializer.h"


enum requestsCodes
{
    REQUEST_LOGIN = 0,
    REQUEST_ERROR = 1,
    REQUEST_SIGNUP = 2
};

enum results
{
    SUCCESS = 0,
    FAIL = 1
};


//i still need to work out the code relations
//Thank you ido for you great cr's I really appreciate it 


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
    
    std::string message = errorResponse.message;
    json messageSerialized = { {"message", message} }; 
    unsigned char errorCode = 1;

    jsonDump = json::to_cbor(messageSerialized);
    numberInBinary = intToBytesVal(jsonDump.size()); 

    buffer.push_back(errorCode);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
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

    int status = loginResponse.status; 
    json statusSerialized = { {"status", status} }; 

    loginCode = status == 0 ? SUCCESS : FAIL;

    jsonDump = json::to_cbor(statusSerialized);
    numberInBinary = intToBytesVal(jsonDump.size()); 

    buffer.push_back(loginCode);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
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

    int status = signupResponse.status; 
    json statusSerialized = { {"status", status} };
    signUpCode = status == 0 ? SUCCESS : FAIL;

    jsonDump = json::to_cbor(statusSerialized); 
    numberInBinary = intToBytesVal(jsonDump.size());

    buffer.push_back(signUpCode);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
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
