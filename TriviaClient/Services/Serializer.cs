using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using PeterO.Cbor;

using Buffer = System.Collections.Generic.List<byte>;
public enum ServerCodes
{
    Login = 0,
    Register = 2,
    SignOut = 3,
    CreateRoom = 21,
    JoinRoom = 22,
    GetRooms = 23,
    GetPlayersInRooms = 24,
    GetPersonalStats = 25,
    GetTop3 = 26,
    CloseRoomReq = 27,
    StartGameReq = 28,
    GetRoomStateReq = 29,
    leaveRoomReq = 30,
    LeaveGameRequest = 40,
    GetQuestionRequest = 41,
    SubmitAnswerRequest = 42,
    GetGameResultsRequest = 43
}

public struct CloseRoomStruct
{}
public struct StartGameStruct
{}
public struct GetRoomStateStruct
{}
public struct leaveRoomStruct
{}
public struct GetQuestionRequest
{}


public struct LoginRequest
{
    public string username { get; set; }
    public string password { get; set; }
}

public struct SignupRequest
{
    public string username { get; set; }
    public string password { get; set; }
    public string email { get; set; }
};

public struct GetPlayersInRoomRequest
{
    public byte roomId { get; set; }
}


public struct CreateRoomRequest
{
    public string roomName { get; set; }
    public byte maxUsers { get; set; }
    public byte questionCount { get; set; }
    public byte answerTimeout { get; set; }
};

public struct GetStatsRequest
{
    public string username { get; set; }
};

public struct GetTop3Request
{}

public struct GetRoomsRequest
{}
public struct JoinRoomRequest
{
    public byte RoomId { get; set; }
}
public struct LeaveGameRequest
{}

public struct SubmitAnswerRequest
{
    public byte answerId { get; set; }
    public int answerTime { get; set; }
}
public struct GetGameResultsRequest
{ }


//we need an external libary for cbor serialization for json named Peter0.Cbor
namespace TriviaClient.Services
{
    public class Serializer
    {
        public Dictionary<Type, ServerCodes> codes;
        public Serializer()
        {
            codes = new Dictionary<Type, ServerCodes>();
            codes.Add(typeof(CreateRoomRequest), ServerCodes.CreateRoom);
            codes.Add(typeof(GetPlayersInRoomRequest), ServerCodes.GetPlayersInRooms);
            codes.Add(typeof(LoginRequest), ServerCodes.Login);
            codes.Add(typeof(SignupRequest), ServerCodes.Register);

            codes.Add(typeof(GetRoomsRequest), ServerCodes.GetRooms);
            codes.Add(typeof(GetStatsRequest), ServerCodes.GetPersonalStats);
            codes.Add(typeof(GetTop3Request), ServerCodes.GetTop3);
            codes.Add(typeof(JoinRoomRequest), ServerCodes.JoinRoom);

            codes.Add(typeof(CloseRoomStruct), ServerCodes.CloseRoomReq);
            codes.Add(typeof(StartGameStruct), ServerCodes.StartGameReq);
            codes.Add(typeof(GetRoomStateStruct), ServerCodes.GetRoomStateReq);
            codes.Add(typeof(leaveRoomStruct), ServerCodes.leaveRoomReq);

            codes.Add(typeof(LeaveGameRequest), ServerCodes.LeaveGameRequest);
            codes.Add(typeof(GetQuestionRequest), ServerCodes.GetQuestionRequest);
            codes.Add(typeof(SubmitAnswerRequest), ServerCodes.SubmitAnswerRequest);
            codes.Add(typeof(GetGameResultsRequest), ServerCodes.GetGameResultsRequest);

        }

        public Buffer SerializeResponse<T>(T requestToServer)
        {
            Buffer buffer = new Buffer();
            String jsonString = JsonSerializer.Serialize(requestToServer);
            byte[] jsonSerialized = CBORObject.FromJSONString(jsonString).EncodeToBytes();
            byte[] jsonLength = BitConverter.GetBytes(jsonSerialized.Length);

            buffer.Add((byte)(codes[typeof(T)])); //add code to buffer

            buffer.AddRange(jsonLength);
            buffer.AddRange(jsonSerialized);
            return buffer;
        }
    }
}
