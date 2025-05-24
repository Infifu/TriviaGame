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
                // === Personal Stats ===
                GetStatsRequest statsReq = new GetStatsRequest
                {
                    username = Client.Instance.nameofuser
                };

                List<byte> statsBuffer = Client.Instance.serializer.SerializeResponse(statsReq);
                ServerAnswer statsAnswer = Client.Instance.communicator.SendAndReceive(statsBuffer);

                GetPersonalStatsResponse statsResponse = JsonSerializer.Deserialize<GetPersonalStatsResponse>(statsAnswer.json);

                foreach (string stat in statsResponse.statistics)
                {
                    if (stat.StartsWith("PlayerScore: "))
                        ScoreValue.Text = stat.Substring("PlayerScore: ".Length);
                    else if (stat.StartsWith("NumOfPlayerGames: "))
                        GamesPlayedValue.Text = stat.Substring("NumOfPlayerGames: ".Length);
                    else if (stat.StartsWith("NumOfTotalAnswers: "))
                        TotalAnswersValue.Text = stat.Substring("NumOfTotalAnswers: ".Length);
                    else if (stat.StartsWith("NumOfCorrectAnswers: "))
                        CorrectAnswersValue.Text = stat.Substring("NumOfCorrectAnswers: ".Length);
                    else if (stat.StartsWith("PlayerAverageAnswerTime: "))
                        AverageTimeValue.Text = stat.Substring("PlayerAverageAnswerTime: ".Length) + "s";
                }

                // === Top 3 ===
                GetTop3Request top3Req = new GetTop3Request();
                List<byte> top3Buffer = Client.Instance.serializer.SerializeResponse(top3Req);
                ServerAnswer top3Answer = Client.Instance.communicator.SendAndReceive(top3Buffer);

                GetHighScoreResponse top3Response = JsonSerializer.Deserialize<GetHighScoreResponse>(top3Answer.json);

                if (top3Response.statistics.Count > 0)
                    Top1Value.Text = top3Response.statistics[0];
                if (top3Response.statistics.Count > 1)
                    Top2Value.Text = top3Response.statistics[1];
                if (top3Response.statistics.Count > 2)
                    Top3Value.Text = top3Response.statistics[2];
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to fetch stats: {ex.Message}");
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
            this.Close();
        }
        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Hide();
        }
    }
}