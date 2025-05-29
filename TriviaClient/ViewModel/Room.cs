using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient.ViewModel
{
    public class Room
    {
        public string Name { get; set; }
        public int PlayerCount { get; set; }
        public int TimeToAnswer { get; set; }
        public void JoinRoom()
        {

        }
        public Room(string name,int playerCount,int timeToAnswer)
        {
            Name = name;
            PlayerCount = playerCount;
            TimeToAnswer = timeToAnswer;
        }
    }
}
