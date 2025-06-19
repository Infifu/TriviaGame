using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using TriviaClient.Services;
using WPFTEST.Services;


public struct GetQuestionResponse
{
    public uint status { get; set; }
    public string question { get; set; }
    public Dictionary<uint, string> answers { get; set; }
}
public struct SubmitAnswerResponse
{
    public uint status { get; set; }
    public uint correctAnswerID { get; set; }
}


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

            QuestionText.Text = questionRes.question;

            var answersList = questionRes.answers.ToList();

            if (answersList.Count >= 4)
            {
                AnswerA.Content = answersList[0].Value;
                AnswerB.Content = answersList[1].Value;
                AnswerC.Content = answersList[2].Value;
                AnswerD.Content = answersList[3].Value;

                AnswerA.Tag = answersList[0].Key;
                AnswerB.Tag = answersList[1].Key;
                AnswerC.Tag = answersList[2].Key;
                AnswerD.Tag = answersList[3].Key;
            }
            else
            {
                QuestionText.Text = "Error: Not enough answers received.";
            }
        }
        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button button)
            {
                uint selectedAnswerId = (uint)button.Tag;

                SubmitAnswerRequest request = new SubmitAnswerRequest
                {
                    answerId = selectedAnswerId 
                };

                List<byte> submitBuffer = Client.Instance.serializer.SerializeResponse(request);
                ServerAnswer serverAnswer = Client.Instance.communicator.SendAndReceive(submitBuffer);

                SubmitAnswerResponse response = JsonSerializer.Deserialize<SubmitAnswerResponse>(serverAnswer.json);

                MessageBox.Show(
                    $"You selected: (ID: {selectedAnswerId})\n" +
                    $"Server Response:\n" +
                    $"- Status: {response.status}\n" +
                    $"- Correct Answer ID: {response.correctAnswerID}",
                    "Answer Submitted",
                    MessageBoxButton.OK,
                    MessageBoxImage.Information
                );
            }
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
