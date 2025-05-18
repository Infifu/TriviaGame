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
using WPFTEST.Services;
namespace TriviaClient.Views
{
    /// <summary>
    /// Interaction logic for LoginRegisterScreen.xaml
    /// </summary>
    public partial class LoginRegisterScreen : Window
    {
        private Communicator communicator;
        public LoginRegisterScreen()
        {
            InitializeComponent();
            communicator = new Communicator();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            communicator.Connect();
        }

        //return the ability to drag the window after removing titlebar
        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if(e.LeftButton==MouseButtonState.Pressed)
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
            communicator.Close();
            Application.Current.Shutdown();
        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Hide();
        }

        private void btnRegister_Click(object sender, RoutedEventArgs e)
        {
        }
        private void btnSwitchToRegister_Click(object sender, RoutedEventArgs e)
        {
            LoginPanel.Visibility = Visibility.Collapsed;
            RegisterPanel.Visibility = Visibility.Visible;
        }
        private void btnSwitchToLogin_Click(object sender, RoutedEventArgs e)
        {
            LoginPanel.Visibility = Visibility.Visible;
            RegisterPanel.Visibility = Visibility.Collapsed;
        }
    }
}
