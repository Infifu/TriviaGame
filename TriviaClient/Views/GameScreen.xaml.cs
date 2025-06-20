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


public class SubmitAnswerResponse
{
    public int status { get; set; }
    public int correctAnswerID { get; set; }
}

public struct GetQuestionResponse
{
    public uint status { get; set; }
    public string question { get; set; }
    public Dictionary<uint, string> answers { get; set; }
}

namespace TriviaClient.Views
{
    public partial class GameScreen : Window
    {
        private int totalQuestions = 10;
        private int questionsLeft = 10;
        private int questionsRight = 0;
        private int answerTimeoutSeconds;
        private DispatcherTimer countdownTimer;
        private int secondsLeft;

        public GameScreen(int timeoutSeconds)
        {
            InitializeComponent();

            answerTimeoutSeconds = timeoutSeconds;
            questionsLeft = 10;
            questionsRight = 0;

            StartTimer();
            LoadNextQuestion();
        }

        private void StartTimer()
        {
            if (countdownTimer != null)
                countdownTimer.Stop();

            secondsLeft = answerTimeoutSeconds;
            TimerText.Text = $"Time: {secondsLeft}s";

            countdownTimer = new DispatcherTimer();
            countdownTimer.Interval = TimeSpan.FromSeconds(1);
            countdownTimer.Tick += CountdownTimer_Tick;
            countdownTimer.Start();
        }

        private void CountdownTimer_Tick(object sender, EventArgs e)
        {
            secondsLeft--;
            TimerText.Text = $"Time: {secondsLeft}s";

            if (secondsLeft <= 0)
            {
                countdownTimer.Stop();
                TimerText.Text = "Time's up!";
                SubmitAnswer(2); 
            }
        }

        private void LoadNextQuestion()
        {
            StartTimer();

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

        private void SubmitAnswer(int selectedAnswerId)
        {
            SubmitAnswerRequest answerReq = new SubmitAnswerRequest();
            answerReq.answerId = (byte)selectedAnswerId;
            List<byte> buffer = Client.Instance.serializer.SerializeResponse(answerReq);
            ServerAnswer answer = Client.Instance.communicator.SendAndReceive(buffer);

            SubmitAnswerResponse response = JsonSerializer.Deserialize<SubmitAnswerResponse>(answer.json);

            if (selectedAnswerId == response.correctAnswerID)
            {
                questionsRight++;
                QuestionsRightText.Text = $"Right: {questionsRight}";
            }

            questionsLeft--;
            QuestionsLeftText.Text = $"Left: {questionsLeft}";

            if (questionsLeft == 0)
            {
                //братанчик сделай ебучую хуйню которая считает авредж время и сунь сюда спс
                EndGameScreen end = new EndGameScreen(totalQuestions,questionsRight,0);
                end.Show();
                this.Close();
            }
            else
            {
                LoadNextQuestion();
            }
        }

        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            int selectedAnswerId = Convert.ToInt32(btn.Tag);
            countdownTimer.Stop();
            SubmitAnswer(selectedAnswerId);
        }

        private void Window_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (e.LeftButton == System.Windows.Input.MouseButtonState.Pressed)
            {
                DragMove();
            }
        }
    }


}
