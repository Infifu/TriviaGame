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
        List<PlayerResults> resultList;

        public EndGameScreen()
        {
            this.totalQuestions = 0;
            this.questionsRight = 0;
            this.averageAnswerTime = 0;
            InitializeComponent();
            this.BackToMenuButton.Visibility = Visibility.Collapsed;
            this.BlockingStatsRectangle.Visibility = Visibility.Visible;
            this.WaitingTextBlock.Visibility = Visibility.Visible;

            Loaded += EndGameScreen_Loaded;

        }

        private async void EndGameScreen_Loaded(object sender, RoutedEventArgs e)
        {
            await EndGameCheck();

            this.BlockingStatsRectangle.Visibility = Visibility.Collapsed;
            this.WaitingTextBlock.Visibility = Visibility.Collapsed;
            this.BackToMenuButton.Visibility = Visibility.Visible;


            this.TotalQuestionsTextBlock.Text = "Total questions: " + this.totalQuestions.ToString();
            this.CorrectAnswersTextBlock.Text = "Total correct answers: " + this.questionsRight.ToString();
            this.AverageAnswerTimeTextBlock.Text = "Average answer time: " + this.averageAnswerTime.ToString();


            List<PlayerResults> SortedList = resultList
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

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Close();
        }


        private async Task EndGameCheck()
        {
            while (true)
            {
                GetGameResultsRequest getGameResultsRequest = new GetGameResultsRequest();
                List<Byte> buffer = Client.Instance.serializer.SerializeResponse(getGameResultsRequest);
                ServerAnswer serverAnswer = Client.Instance.communicator.SendAndReceive(buffer);
                GetGameResultsAnswer resultRes = JsonSerializer.Deserialize<GetGameResultsAnswer>(serverAnswer.json);

                if (resultRes.status == 0)
                {
                    foreach (var player in resultRes.results)
                    {
                        if (player.username == Client.Instance.nameofuser)
                        {
                            this.averageAnswerTime = player.averageAnswerTime;
                            this.totalQuestions = player.correctAnswerCount + player.wrongAnswerCount;
                            this.questionsRight = player.correctAnswerCount;
                            this.resultList = resultRes.results;
                        }
                    }
                    break;
                }
                await Task.Delay(3000);
            }
        }
    }
}
