using System;
using System.Net.Sockets;
using System.Net;

namespace WPFTEST.Services
{
    public class Communicator
    {
        private Socket _socket;

        public Communicator()
        {
            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }

        public void Connect(string ip = "127.0.0.1", int port = 6969)
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