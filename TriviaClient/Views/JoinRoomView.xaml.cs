using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using TriviaClient.ViewModel;

namespace TriviaClient.Views
{
    /// <summary>
    /// Логика взаимодействия для JoinRoomView.xaml
    /// </summary>
    public partial class JoinRoomView : Window
    {
        public ObservableCollection<Room> Rooms { get; set; }
        public JoinRoomView()
        {
            InitializeComponent();

            Rooms = new ObservableCollection<Room>
            {
                new Room("SagiRoom", 6,30),
                new Room("ShmuelRoom", 4,20),
                new Room("OriRoom", 4,10)
            };

            // Set DataContext for data binding
            DataContext = this;
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

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            Room selectedRoom = btn.Tag as Room;

            if (selectedRoom != null)
            {
                RoomPlayerInfoPanell.Visibility = Visibility.Visible;
                RoomNameShow.Text = "Room name: " + selectedRoom.Name;
                PlayerAmountShow.Text = "Player amount: " + selectedRoom.PlayerCount.ToString();
                TimeToAnswerShow.Text = "Time to answer (seconds): " + selectedRoom.TimeToAnswer.ToString();
            }


        }

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Hide();
        }
    }
}