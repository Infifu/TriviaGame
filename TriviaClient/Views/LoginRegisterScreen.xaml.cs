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
using TriviaClient.Services;
using WPFTEST.Services;
namespace TriviaClient.Views
{
    /// <summary>
    /// Interaction logic for LoginRegisterScreen.xaml
    /// </summary>
    public partial class LoginRegisterScreen : Window
    {
        private Communicator communicator;
        private Serializer serializer;

        public LoginRegisterScreen()
        {
            InitializeComponent();
            communicator = new Communicator();
            serializer = new Serializer();

            Thread thread = new Thread(() =>
            {
                try
                {
                    communicator.Connect();
                    Console.WriteLine("Connected");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Connection failed: " + ex.Message);
                }
            });

            thread.IsBackground = true;
            thread.Start();
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
            if (txtUsername.Text == "" || txtPAssword.Password == "")
            {
                ErrorBox.Visibility = Visibility.Visible;
                ErrorBox.Text = "Please fill all the fields";
            }
            else
            {
                LoginRequest logReq = new LoginRequest();
                logReq.username = txtUsername.Text;
                logReq.password = txtPAssword.Password;

                //might freeze when waiting for answer
                ServerAnswer answer = communicator.SendAndReceive(serializer.SerializeResponse(logReq));
                if (answer.code == 0)
                {
                    PlayerData.username = txtUsername.Text;
                    MainMenu mainMenu = new MainMenu();
                    mainMenu.Show();
                    this.Hide();
                }
                else
                {
                    ErrorBox.Visibility = Visibility.Visible;
                    ErrorBox.Text = "Error login";
                }    
            }
        }

        private void btnRegister_Click(object sender, RoutedEventArgs e)
        {
            //if (txtUsernameInReg.Text == "" || txtPAsswordInReg.Password == "" || txtEmailInReg.Text == "")
            //{
            //    ErrorBox.Visibility = Visibility.Visible;
            //    ErrorBox.Text = "Please fill all the fields";
            //}
            //else
            //{
            //    SignupRequest signUpReq = new SignupRequest();
            //    signUpReq.username = txtUsernameInReg.Text;
            //    signUpReq.password = txtPAsswordInReg.Password;
            //    signUpReq.email = txtEmailInReg.Text;

            //    //might freeze when waiting for answer
            //    ServerAnswer answer = communicator.SendAndReceive(serializer.SerializeResponse(signUpReq));
            //    if (answer.code == 0)
            //    {
            //        ErrorBox.Visibility = Visibility.Collapsed;
            //        LoginPanel.Visibility = Visibility.Visible;
            //        RegisterPanel.Visibility = Visibility.Collapsed;
            //    }
            //    else
            //    {
            //        ErrorBox.Visibility = Visibility.Visible;
            //        ErrorBox.Text = "Register error";
            //    }
            //}
        }
        private void btnSwitchToRegister_Click(object sender, RoutedEventArgs e)
        {
            ErrorBox.Visibility = Visibility.Collapsed;
            LoginPanel.Visibility = Visibility.Collapsed;
            RegisterPanel.Visibility = Visibility.Visible;
        }
        private void btnSwitchToLogin_Click(object sender, RoutedEventArgs e)
        {
            ErrorBox.Visibility = Visibility.Collapsed;
            LoginPanel.Visibility = Visibility.Visible;
            RegisterPanel.Visibility = Visibility.Collapsed;
        }
    }
}
