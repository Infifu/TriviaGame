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

Buffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    int status = response.status;
    json statusSerialized = { {"status", status} };

    unsigned char logOutcode = response.status; //here the codes match (for now)

    jsonDump = json::to_cbor(statusSerialized);
    numberInBinary = intToBytesVal(jsonDump.size());

    buffer.push_back(logOutcode);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    std::vector<json> jsonVector;
    
    for (auto const& room : response.rooms)
    {
        json j;
        j["id"] = room.id;
        j["name"] = room.name;
        j["maxPlayers"] = room.maxPlayers;
        j["numOfQuestions"] = room.numOfQuestionsInGame;
        j["timePerQuestion"] = room.timePerQuestion;
        j["status"] = room.status;
        jsonVector.push_back(j);
    }
 
    int status = response.status; //make the status match the code, untill i figure out
    json statusSerialized = { {"status", response.status}, {"rooms", jsonVector} };

    jsonDump = json::to_cbor(statusSerialized);
    numberInBinary = intToBytesVal(jsonDump.size());

    buffer.push_back(status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    json statusSerialized = { {"players" , response.players} };

    jsonDump = json::to_cbor(statusSerialized);
    numberInBinary = intToBytesVal(jsonDump.size());

    buffer.push_back(SUCCESS); //success for now
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    int status = response.status;
    json statusSerialized = { {"status", status} };
    unsigned char roomResCode = response.status; //here the codes match (for now)

    jsonDump = json::to_cbor(statusSerialized);
    numberInBinary = intToBytesVal(jsonDump.size());

    buffer.push_back(roomResCode);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    int status = response.status;
    json statusSerialized = { {"status", status} };

    unsigned char roomResponseCode = response.status; //here the codes match (for now)

    jsonDump = json::to_cbor(statusSerialized);
    numberInBinary = intToBytesVal(jsonDump.size());

    buffer.push_back(roomResponseCode);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
    Buffer jsonDump;
    Buffer buffer;
    Buffer numberInBinary;

    json statusSerialized = { {"status", response.status} , {"statistics" , response.statistics} };

    unsigned char highScoreCode = response.status; //here the codes match (for now)

    jsonDump = json::to_cbor(statusSerialized);
    numberInBinary = intToBytesVal(jsonDump.size());

    buffer.push_back(highScoreCode);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
        Buffer jsonDump;
        Buffer buffer;
        Buffer numberInBinary;

        json statusSerialized = { {"status", response.status} , {"statistics" , response.statistics} };

        unsigned char personalStatCode = response.status; //here the codes match (for now)

        jsonDump = json::to_cbor(statusSerialized);
        numberInBinary = intToBytesVal(jsonDump.size());

        buffer.push_back(personalStatCode);
        buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
        buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
        return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& response)
{
    json statusSerialized = { {"status", response.status} };
    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& response)
{
    json statusSerialized = { {"status", response.status} };
    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& response)
{
    json statusSerialized = { {"status", response.status} };
    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& response)
{
    json statusSerialized = {
        {"status", response.status},
        {"hasGameBegun", response.hasGameBegun},
        {"players", response.players},
        {"AnswerCount", response.answerCount},
        {"answerTimeOut", response.answerTimeout}
    };

    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& response)
{
    json statusSerialized;
    statusSerialized["status"] = response.status;
    json playersJsonArray = json::array(); //array for the players

    for (const PlayerResults& player : response.results)
    {
        json playerJson;
        playerJson["username"] = player.username;
        playerJson["correctAnswerCount"] = player.correctAnswerCount;
        playerJson["wrongAnswerCount"] = player.wrongAnswerCount;
        playerJson["averageAnswerTime"] = player.averageAnswerTime;
        playersJsonArray.push_back(playerJson);
    }
    statusSerialized["results"] = playersJsonArray;

    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& response)
{
    json statusSerialized = {
        {"status", response.status},
        {"correctAnswerID", response.correctAnswerID}};

    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& response)
{
    json statusSerialized = {
     {"status", response.status},
     {"question", response.question},
     {"answers",response.answers} };

    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
    buffer.insert(buffer.end(), numberInBinary.begin(), numberInBinary.end());
    buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());
    return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& response)
{
    json statusSerialized = {{"status", response.status}};

    Buffer jsonDump = json::to_cbor(statusSerialized);
    Buffer numberInBinary = intToBytesVal(jsonDump.size());

    Buffer buffer;
    buffer.push_back(response.status);
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
