using Statistics.View;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace TriviaClient.Views
{
    /// <summary>
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Window
    {
        public MainMenu()
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

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void btnStatistics_Click(object sender, RoutedEventArgs e)
        {
            StatisticsView statistics = new StatisticsView();
            statistics.Show();
            this.Hide();
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoomScreen createRoom = new CreateRoomScreen();
            createRoom.Show();
            this.Hide();
        }

        private void btnJoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoomView joinRoomView = new JoinRoomView();
            joinRoomView.Show();
            this.Hide();
        }
    }
}
