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
                GetStatsRequest req = new GetStatsRequest
                {
                    username = Client.Instance.nameofuser
                };

                List<byte> serializedRequest = Client.Instance.serializer.SerializeResponse(req);
                ServerAnswer answer = Client.Instance.communicator.SendAndReceive(serializedRequest);

                GetPersonalStatsResponse response = JsonSerializer.Deserialize<GetPersonalStatsResponse>(answer.json);
                StatisticsList.ItemsSource = response.statistics;
            }
            catch (System.Exception ex)
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