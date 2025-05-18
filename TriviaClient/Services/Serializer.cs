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
    Login = 1,
    Register = 2,
    SignOut = 3,
    CreateRoom = 21,
    JoinRoom = 22,
    GetRooms = 23,
    GetPlayersInRooms = 24,
    GetPersonalStats = 25,
    GetHighScores = 26
}

struct LoginRequest
{
    string username;
    string password;
};

struct SignupRequest
{
    string username;
    string password;
    string email;
};

struct GetPlayersInRoomRequest
{
    byte roomId;
};

struct JoinRoomRequest
{
    byte roomId;
};

struct CreateRoomRequest
{
    string roomName;
    byte maxUsers;
    byte questionCount;
    byte answerTimeout;
};

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
            codes.Add(typeof(JoinRoomRequest), ServerCodes.JoinRoom);
            codes.Add(typeof(GetPlayersInRoomRequest), ServerCodes.GetPlayersInRooms);
            codes.Add(typeof(LoginRequest), ServerCodes.Login);
            codes.Add(typeof(SignupRequest), ServerCodes.Register);

            //codes.Add(typeof(GetRoomsRequest), ServerCodes.CreateRoom); -- No get rooms request
            //codes.Add(typeof(GetPersonalStatsRequest), ServerCodes.GetPersonalStats); -- No personal stats request
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
