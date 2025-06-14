using System;
using System.Net.Sockets;
using System.Net;
using Buffer = System.Collections.Generic.List<byte>;
using System.Security.Cryptography;
using TriviaClient.Services;
using System.Text.Json;
using PeterO.Cbor;
using System.Windows;


namespace WPFTEST.Services
{
    public struct ServerAnswer
    {
        public int code;
        public int length;
        public string json;
    }


    public class Communicator
    {
        private Socket _socket;
        private const int port = 6969;
        private const string ip = "127.0.0.1";

        public Communicator()
        {
            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }

        public void Connect()
        {
            try
            {
                IPEndPoint remoteEP = new IPEndPoint(IPAddress.Parse(ip), port);
                _socket.Connect(remoteEP);
                Console.WriteLine("Connected to server");
            }
            catch (SocketException ex)
            {
                Console.WriteLine(ex.Message);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        public ServerAnswer SendAndReceive(Buffer msg)
        {
            ServerAnswer answer = new ServerAnswer();

            byte[] codeInBytes = new byte[1];
            byte[] lengthInBytes = new byte[4];

            if (!_socket.Connected)
            {
                MessageBox.Show("ERROR");
                Environment.Exit(1); // Exit if not connected to server
            }

            _socket.Send(msg.ToArray());

            int received = _socket.Receive(codeInBytes, 0, 1, SocketFlags.None);
            if (received != 1)
            {
                throw new Exception("Error while receiving response code.");
            }
            answer.code = codeInBytes[0];

            received = _socket.Receive(lengthInBytes, 0, 4, SocketFlags.None);
            if (received != 4)
            {
                throw new Exception("Error while receiving length.");
            }
            answer.length = BitConverter.ToInt32(lengthInBytes, 0);

            byte[] jsonInCbor = new byte[answer.length];
            received = _socket.Receive(jsonInCbor, 0, answer.length, SocketFlags.None);
            if (received != answer.length)
            {
                throw new Exception("Error while receiving CBOR data.");
            }

            CBORObject jsonFromCbor = CBORObject.DecodeFromBytes(jsonInCbor);
            answer.json = jsonFromCbor.ToJSONString();

            return answer;
        }



        public void Close()
        {
            if (_socket.Connected)
            {
                _socket.Shutdown(SocketShutdown.Both);
                _socket.Close();
            }
        }
    }
}