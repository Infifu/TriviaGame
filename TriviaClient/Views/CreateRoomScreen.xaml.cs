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
    /// Interaction logic for CreateRoomScreen.xaml
    /// </summary>
    public partial class CreateRoomScreen : Window
    {
        public CreateRoomScreen()
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

        private void btnStartGame_Click(object sender, RoutedEventArgs e)
        {

        }

        private void txtTimeToAnswer_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !int.TryParse(e.Text, out _); // Allow only numbers
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            if (PlayerAmountBox.SelectedIndex == -1 || txtRoomname.Text == "" || txtTimeToAnswer.Text == "")
            {
                ErrorMessage.Visibility = Visibility.Visible;
            }
            else
            {
                CreateRoomPanel.Visibility = Visibility.Collapsed;
                RoomPlayerInfoPanell.Visibility = Visibility.Visible;

                TimeToAnswerShow.Text = "Time to answer: " + txtTimeToAnswer.Text;
                PlayerAmountShow.Text = "Player amount: " + PlayerAmountBox.Text;
                RoomNameShow.Text = "Room Name: " + txtRoomname.Text;

                btnStartGame.Visibility = Visibility.Visible;
            }
        }
    }
}
