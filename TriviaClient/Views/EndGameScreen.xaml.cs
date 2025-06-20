using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
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
    public struct PlayerResults
    {
        public string username { get; set; }
        public byte correctAnswerCount { get; set; }
        public byte wrongAnswerCount { get; set; }
        public byte averageAnswerTime { get; set; }
    }

    public struct GetGameResultsAnswer
    {
        public byte status { get; set; }
        public List<PlayerResults> results { get; set; }
    }




    /// <summary>
    /// Interaction logic for EndGameScreen.xaml
    /// </summary>
    public partial class EndGameScreen : Window
    {
        private int totalQuestions;
        private int questionsRight;
        private int averageAnswerTime;

        public EndGameScreen(int totalQus,int RightQus,int averageAnswerTime)
        {
            this.totalQuestions = totalQus;
            this.questionsRight = RightQus;
            this.averageAnswerTime = averageAnswerTime;
            InitializeComponent();                                                                  

            this.TotalQuestionsTextBlock.Text = "Total questions: " + this.totalQuestions.ToString();
            this.CorrectAnswersTextBlock.Text = "Total correct answers: " + this.questionsRight.ToString();
            this.AverageAnswerTimeTextBlock.Text = "Average answer time: " + this.averageAnswerTime.ToString();

            GetGameResultsRequest getGameResultsRequest = new GetGameResultsRequest();
            List<Byte> buffer = Client.Instance.serializer.SerializeResponse(getGameResultsRequest);
            ServerAnswer serverAnswer = Client.Instance.communicator.SendAndReceive(buffer);
            GetGameResultsAnswer resultRes = JsonSerializer.Deserialize<GetGameResultsAnswer>(serverAnswer.json);

            List<PlayerResults> SortedList = resultRes.results
                .OrderByDescending(o => o.correctAnswerCount)
                .ToList();

            if (SortedList.Count > 0)
                this.FirstPlaceTextBlock.Text = SortedList[0].username;

            if (SortedList.Count > 1)
                this.SecondPlaceTextBlock.Text = SortedList[1].username;

            if (SortedList.Count > 2)
                this.ThirdPlaceTextBlock.Text = SortedList[2].username;

        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                this.DragMove();
            }
        }

        private void btnMinimize_Click(object sender, RoutedEventArgs e)
        {
            this.WindowState = WindowState.Minimized;
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }


    }
}
