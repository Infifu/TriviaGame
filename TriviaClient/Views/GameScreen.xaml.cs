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
        private int questionsLeft = 20;
        private int questionsRight = 0;

        public GameScreen()
        {
            InitializeComponent();

            QuestionsLeftText.Text = $"Left: {questionsLeft}";
            QuestionsRightText.Text = $"Right: {questionsRight}";

            LoadNextQuestion();
        }

        private void LoadNextQuestion()
        {
            GetQuestionRequest questionReq = new GetQuestionRequest();
            List<byte> buffer = Client.Instance.serializer.SerializeResponse(questionReq);
            ServerAnswer serverAnswer = Client.Instance.communicator.SendAndReceive(buffer);

            GetQuestionResponse questionRes = JsonSerializer.Deserialize<GetQuestionResponse>(serverAnswer.json);

            QuestionText.Text = questionRes.question;

            var answers = questionRes.answers.ToList();

            AnswerA.Content = answers[0].Value;
            AnswerA.Tag = answers[0].Key;

            AnswerB.Content = answers[1].Value;
            AnswerB.Tag = answers[1].Key;

            AnswerC.Content = answers[2].Value;
            AnswerC.Tag = answers[2].Key;

            AnswerD.Content = answers[3].Value;
            AnswerD.Tag = answers[3].Key;

            AnswerA.IsEnabled = true;
            AnswerB.IsEnabled = true;
            AnswerC.IsEnabled = true;
            AnswerD.IsEnabled = true;
        }

        private async void Answer_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button button)
            {
                uint selectedAnswerId = (uint)button.Tag;

                AnswerA.IsEnabled = false;
                AnswerB.IsEnabled = false;
                AnswerC.IsEnabled = false;
                AnswerD.IsEnabled = false;

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

                if (selectedAnswerId == response.correctAnswerID)
                {
                    questionsRight++;
                }
                questionsLeft--;

                QuestionsRightText.Text = $"Right: {questionsRight}";
                QuestionsLeftText.Text = $"Left: {questionsLeft}";

                if (questionsLeft == 0)
                {
                    EndGameScreen endScreen = new EndGameScreen();
                    endScreen.Show();
                    this.Close();
                    return;
                }

                await Task.Delay(1000);

                LoadNextQuestion();
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
