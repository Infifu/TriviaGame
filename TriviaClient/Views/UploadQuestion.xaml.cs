using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Nodes;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using TriviaClient.Services;
using WPFTEST.Services;

namespace TriviaClient.Views
{
    public struct UploadQuestionResponse
    {
        public byte status { get; set; }
    }


    /// <summary>
    /// Interaction logic for UploadQuestion.xaml
    /// </summary>
    public partial class UploadQuestion : Window
    {
        public UploadQuestion()
        {
            InitializeComponent();
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void btnMinimize_Click(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void btunUpload_Click(object sender, RoutedEventArgs e)
        {
            if (firstAnswerTextBlock.Text.Length > 0 && secondAnswerTextBlock.Text.Length > 0 && thirdAnswerTextBlock.Text.Length > 0
                && fourthAnswerTextBlock.Text.Length > 0 && correctAnswerTextBox.Text.Length > 0 && QuestionTextBox.Text.Length > 0)
            {
                List<String> answers = new List<String>();
                answers.Add(firstAnswerTextBlock.Text.Trim());
                answers.Add(secondAnswerTextBlock.Text.Trim());
                answers.Add(thirdAnswerTextBlock.Text.Trim());
                answers.Add(fourthAnswerTextBlock.Text.Trim());

                Random rng = new Random();
                int n = answers.Count;
                while (n > 1)
                {
                    n--;
                    int k = rng.Next(n + 1);
                    (answers[n], answers[k]) = (answers[k], answers[n]);
                }

                UploadQuestionRequest uploadQuestionRequest = new UploadQuestionRequest();
                uploadQuestionRequest.question = QuestionTextBox.Text.Trim();
                uploadQuestionRequest.answerOne = answers[0];
                uploadQuestionRequest.answerTwo = answers[1];
                uploadQuestionRequest.answerThree = answers[2];
                uploadQuestionRequest.answerFour = answers[3];
                int answerId = answers.IndexOf(correctAnswerTextBox.Text.Trim());
                if (answerId != -1)
                {
                    uploadQuestionRequest.correctAnswerID = (byte)answers.IndexOf(correctAnswerTextBox.Text.Trim());
                    List<Byte> buffer = Client.Instance.serializer.SerializeResponse(uploadQuestionRequest);
                    ServerAnswer serverAnswer = Client.Instance.communicator.SendAndReceive(buffer);
                    UploadQuestionResponse response = JsonSerializer.Deserialize<UploadQuestionResponse>(serverAnswer.json);
                    if (response.status == 1)
                    {
                        this.errorBox.Text = "Failed";
                    }
                    else if (response.status == 0)
                    {
                        this.errorBox.Text = "Success";
                    }
                }
            }
        }

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Hide();
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            SignOut leaveGameRequest = new SignOut();
            List<Byte> buffer = Client.Instance.serializer.SerializeResponse(leaveGameRequest);
            ServerAnswer serverAnswer = Client.Instance.communicator.SendAndReceive(buffer);

            Application.Current.Shutdown();
        }
    }
}
