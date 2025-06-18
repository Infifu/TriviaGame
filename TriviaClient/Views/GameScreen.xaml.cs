using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using TriviaClient.Services;
using WPFTEST.Services;
public struct GetQuestionRequest { }

public struct GetQuestionResponse
{
    public uint status { get; set; }
    public string question { get; set; }
    public Dictionary<uint, string> answers { get; set; }
}

public struct SubmitAnswerRequest
{
    public uint answerId { get; set; }
}

public struct LeaveGameRequest { }


namespace TriviaClient.Views
{
    public partial class GameScreen : Window
    {

        public GameScreen()
        {
            InitializeComponent();
        }
    }
}
