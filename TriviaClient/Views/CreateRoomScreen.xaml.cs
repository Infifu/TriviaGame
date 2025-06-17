using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    /// <summary>
    /// Interaction logic for CreateRoomScreen.xaml
    /// </summary>
    public partial class CreateRoomScreen : Window
    {
        public ObservableCollection<string> Players { get; set; }

        Task refreshMembers;
        bool is_refreshing;

        private async Task RefreshMembersListLoopAsync()
        {
            while (is_refreshing)
            {
                await RefreshMembersList();
                await Task.Delay(3000);
            }
        }

        private async Task RefreshMembersList()
        {
            try
            {
                GetRoomStateStruct getRoomStatereq = new GetRoomStateStruct();
                List<byte> playerReqBuffer = Client.Instance.serializer.SerializeResponse(getRoomStatereq);
                ServerAnswer playerAnswer = Client.Instance.communicator.SendAndReceive(playerReqBuffer);

                GetRoomStateResponse playerRes = JsonSerializer.Deserialize<GetRoomStateResponse>(playerAnswer.json);
                if (playerRes != null && playerRes.players != null)
                {
                    Players.Clear();
                    foreach (var player in playerRes.players)
                    {
                        Players.Add(player);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error in refreshMembersList: " + ex.Message);
            }
        }

        public CreateRoomScreen()
        {
            InitializeComponent();
            Players = new ObservableCollection<string> { };
            DataContext = this;

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
            try
            {
                StartGameStruct startGameReq = new StartGameStruct();
                List<byte> startGameBuffer = Client.Instance.serializer.SerializeResponse(startGameReq);
                ServerAnswer startGameAnswer = Client.Instance.communicator.SendAndReceive(startGameBuffer);

                if (startGameAnswer.code == 0)
                {
                    MessageBox.Show("Game started successfully!");
                }
                else
                {
                    MessageBox.Show("Failed to start game.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error sending start game request: " + ex.Message);
            }
        }


        private void txtTimeToAnswer_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            e.Handled = !int.TryParse(e.Text, out _); // allow only numbers
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            if (PlayerAmountBox.SelectedIndex == -1 || txtRoomname.Text == "" || txtTimeToAnswer.Text == "")
            {
                ErrorMessage.Visibility = Visibility.Visible;
            }
            else
            {
                CreateRoomRequest roomReq = new CreateRoomRequest();
                roomReq.roomName = txtRoomname.Text;
                roomReq.maxUsers = (Byte)Int32.Parse(PlayerAmountBox.Text);
                roomReq.answerTimeout = (Byte)Int32.Parse(txtTimeToAnswer.Text);
                roomReq.questionCount = (Byte)Int32.Parse("20");

                ServerAnswer answer = Client.Instance.communicator.SendAndReceive(Client.Instance.serializer.SerializeResponse(roomReq));
                
                if (answer.code == 0)
                {
                    CreateRoomPanel.Visibility = Visibility.Collapsed;
                    RoomPlayerInfoPanell.Visibility = Visibility.Visible;

                    TimeToAnswerShow.Text = "Time to answer: " + txtTimeToAnswer.Text;
                    PlayerAmountShow.Text = "Player amount: " + PlayerAmountBox.Text;
                    RoomNameShow.Text = "Room Name: " + txtRoomname.Text;
                    Players.Add(Client.Instance.nameofuser);

                    btnStartGame.Visibility = Visibility.Visible;

                    is_refreshing = true;
                    refreshMembers = RefreshMembersListLoopAsync();

                }
                else
                {
                    //add here an error message
                }
            }
        }
        private void closeRoom()
        {
            CloseRoomStruct closeRoomReq = new CloseRoomStruct();
            List<byte> leaveRoomBuffer = Client.Instance.serializer.SerializeResponse(closeRoomReq);
            ServerAnswer leaveRoomAnswer = Client.Instance.communicator.SendAndReceive(leaveRoomBuffer);
        }

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            if (is_refreshing)
            {
                is_refreshing = false;
                closeRoom();
            }
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Hide();
        }
    }
}
