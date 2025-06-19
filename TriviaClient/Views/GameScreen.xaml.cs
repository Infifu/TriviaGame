using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Threading;
using TriviaClient.Services;
using WPFTEST.Services;


public struct GetQuestionResponse
{
    public uint status { get; set; }
    public string question { get; set; }
    public Dictionary<uint, string> answers { get; set; }
}

//public struct SubmitAnswerRequest
//{
//    public uint answerId { get; set; }
//}

namespace TriviaClient.Views
{
    public partial class GameScreen : Window
    {

        public GameScreen()
        {
            InitializeComponent();

            GetQuestionRequest questionReq = new GetQuestionRequest();
            List<byte> startGameBuffer = Client.Instance.serializer.SerializeResponse(questionReq);
            ServerAnswer serverAnswer = Client.Instance.communicator.SendAndReceive(startGameBuffer);

            GetQuestionResponse questionRes = JsonSerializer.Deserialize<GetQuestionResponse>(serverAnswer.json);
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }

           
        }
    }
}
