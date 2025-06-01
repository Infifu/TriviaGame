using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WPFTEST.Services;

//singleton class for client
namespace TriviaClient.Services
{
    public sealed class Client
    {
        private static readonly Client instance = new Client();

        // Explicit static constructor to tell C# compiler
        // not to mark type as beforefieldinit
        static Client()
        {
        }

        private Client()
        {
            communicator = new Communicator();
            serializer = new Serializer();
            deserializer = new Deserializer();
        }

        public Communicator communicator { get; }
        public Serializer serializer { get; }
        public Deserializer deserializer { get; }
        
        public String nameofuser { get; set; }



        public static Client Instance
        {
            get
            {
                return instance;
            }
        }
    }
}