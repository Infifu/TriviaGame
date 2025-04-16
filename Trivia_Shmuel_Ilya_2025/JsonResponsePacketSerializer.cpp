#include "JsonResponsePacketSerializer.h"

/**
 * @brief serialize error response (converting to binary)
 * @param errorResponse - struct that holds the message
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse errorResponse)
{
    std::string message = errorResponse.message;
    Buffer buffer;
    json jstatus = { {"message" , message} };
    buffer = json::to_cbor(jstatus);
    return buffer;
}

/**
 * @brief serialize login response (converting to binary)
 * @param loginResponse - struct that holds the status
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse loginResponse)
{
    unsigned int status = loginResponse.status;
    Buffer buffer;
    json jstatus = { {"status" , status} };
    buffer = json::to_cbor(jstatus);
    return buffer; 
}

/**
 * @brief serialize signup response (converting to binary)
 * @param loginResponse - struct that holds the status
 * @return the buffers (vector of unsigned char)
 */
Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse signupResponse)
{
    unsigned int status = signupResponse.status;
    Buffer buffer;
    json jstatus = { {"status" , status} };
    buffer = json::to_cbor(jstatus);
    return buffer;
}
