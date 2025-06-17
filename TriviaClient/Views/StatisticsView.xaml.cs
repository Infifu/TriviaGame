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
using TriviaClient.Views;
using WPFTEST.Services;

namespace TriviaClient.Services
{
    public class GetPersonalStatsResponse
    {
        public int status { get; set; }
        public List<string> statistics { get; set; }
    }
}
public class GetHighScoreResponse
{
    public int status { get; set; }
    public List<string> statistics { get; set; }
}


namespace Statistics.View
{
    /// <summary>
    /// Логика взаимодействия для StatisticsView.xaml
    /// </summary>
    public partial class StatisticsView : Window
    {
        public StatisticsView()
        {
            InitializeComponent();
            Loaded += StatisticsView_Loaded;
        }

        private void StatisticsView_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                GetStatsRequest statsReq = new GetStatsRequest
                {
                    username = Client.Instance.nameofuser
                };

                List<byte> statsBuffer = Client.Instance.serializer.SerializeResponse(statsReq);
                ServerAnswer statsAnswer = Client.Instance.communicator.SendAndReceive(statsBuffer);
                GetPersonalStatsResponse statsResponse = JsonSerializer.Deserialize<GetPersonalStatsResponse>(statsAnswer.json);

                string userScoreStr = "";
                foreach (string stat in statsResponse.statistics)
                {
                    if (stat.StartsWith("PlayerScore: "))
                    {
                        userScoreStr = stat.Substring("PlayerScore: ".Length).Trim();
                        ScoreValue.Text = userScoreStr;
                    }
                    else if (stat.StartsWith("NumOfPlayerGames: "))
                        GamesPlayedValue.Text = stat.Substring("NumOfPlayerGames: ".Length);
                    else if (stat.StartsWith("NumOfTotalAnswers: "))
                        TotalAnswersValue.Text = stat.Substring("NumOfTotalAnswers: ".Length);
                    else if (stat.StartsWith("NumOfCorrectAnswers: "))
                        CorrectAnswersValue.Text = stat.Substring("NumOfCorrectAnswers: ".Length);
                    else if (stat.StartsWith("PlayerAverageAnswerTime: "))
                        AverageTimeValue.Text = stat.Substring("PlayerAverageAnswerTime: ".Length) + "s";
                }

                GetTop3Request top3Req = new GetTop3Request();
                List<byte> top3Buffer = Client.Instance.serializer.SerializeResponse(top3Req);
                ServerAnswer top3Answer = Client.Instance.communicator.SendAndReceive(top3Buffer);
                GetHighScoreResponse top3Response = JsonSerializer.Deserialize<GetHighScoreResponse>(top3Answer.json);

                string top1Name = "";
                string top3ScoreStr = "0";

                if (top3Response.statistics.Count > 0)
                {
                    string[] parts = top3Response.statistics[0].Split(':');
                    top1Name = parts[0];
                    Top1Value.Text = top1Name;
                    Top1ScoreValue.Text = "Top 1 = " + parts[1];
                }

                if (top3Response.statistics.Count > 1)
                {
                    string[] parts = top3Response.statistics[1].Split(':');
                    Top2Value.Text = parts[0];
                    Top2ScoreValue.Text = "Top 2 = " + parts[1];
                }

                if (top3Response.statistics.Count > 2)
                {
                    string[] parts = top3Response.statistics[2].Split(':');
                    Top3Value.Text = parts[0];
                    Top3ScoreValue.Text = "Top 3 = " + parts[1];
                    top3ScoreStr = parts[1];
                }

                if (top1Name != "")
                {
                    GetStatsRequest top1StatsReq = new GetStatsRequest { username = top1Name };
                    List<byte> top1StatsBuffer = Client.Instance.serializer.SerializeResponse(top1StatsReq);
                    ServerAnswer top1StatsAnswer = Client.Instance.communicator.SendAndReceive(top1StatsBuffer);
                    GetPersonalStatsResponse top1Stats = JsonSerializer.Deserialize<GetPersonalStatsResponse>(top1StatsAnswer.json);

                    foreach (string stat in top1Stats.statistics)
                    {
                        if (stat.StartsWith("PlayerScore: "))
                            Top1ScoreStatValue.Text = stat.Substring("PlayerScore: ".Length);
                        else if (stat.StartsWith("NumOfPlayerGames: "))
                            Top1GamesPlayedStatValue.Text = stat.Substring("NumOfPlayerGames: ".Length);
                        else if (stat.StartsWith("NumOfTotalAnswers: "))
                            Top1TotalAnswersStatValue.Text = stat.Substring("NumOfTotalAnswers: ".Length);
                        else if (stat.StartsWith("NumOfCorrectAnswers: "))
                            Top1CorrectAnswersStatValue.Text = stat.Substring("NumOfCorrectAnswers: ".Length);
                        else if (stat.StartsWith("PlayerAverageAnswerTime: "))
                            Top1AverageTimeStatValue.Text = stat.Substring("PlayerAverageAnswerTime: ".Length) + "s";
                    }
                }

                int userScore = int.Parse(userScoreStr);
                int top3Score = int.Parse(top3ScoreStr);

                if (top3Score > userScore)
                {
                    PointsToPodiumText.Text = "You need " + (top3Score - userScore) + " more points to get on podium";
                }
                else
                {
                    PointsToPodiumText.Text = "You are on the podium!";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to fetch stats: " + ex.Message);
            }
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
            this.Hide();
        }
    }
}