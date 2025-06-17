using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics.Eventing.Reader;
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
using TriviaClient.ViewModel;
using WPFTEST.Services;
using System.Threading;

public class Room
{
    public int Id { get; set; }
    public string Name { get; set; }
    public int PlayerCount { get; set; }
    public int TimeToAnswer { get; set; }
    public string PlayerStatus => $"Players: {PlayerCount}";

    public Room(string name, int playerCount, int timeToAnswer, int id)
    {
        Name = name;
        PlayerCount = playerCount;
        TimeToAnswer = timeToAnswer;
        Id = id;
    }
}

public class RoomData
{
    public int id { get; set; }
    public string name { get; set; }
    public int maxPlayers { get; set; }
    public int timePerQuestion { get; set; }
    public int numOfQuestionsInGame { get; set; }
    public int status { get; set; }
}

public class GetRoomsResponse
{
    public int status { get; set; }
    public List<RoomData> rooms { get; set; }
}
public class GetPlayersInRoomResponse
{
    public List<string> players { get; set; }
}

public class GetRoomStateResponse
{
    public int status { get; set; }
    public bool hasGameBegun { get; set; }
    public List<string> players { get; set; }
    public int AnswerCount { get; set; }
    public int answerTimeOut { get; set; }
}

namespace TriviaClient.Views
{
    /// <summary>
    /// Логика взаимодействия для JoinRoomView.xaml
    /// </summary>
    public partial class JoinRoomView : Window
    {
        public ObservableCollection<Room> Rooms { get; set; }
        public ObservableCollection<string> Players { get; set; }

        Task loadRoomsTask;
        bool is_loading;

        Task refreshPlayerListTask;
        bool is_refreshing;


        public JoinRoomView()
        {
            InitializeComponent();

            Rooms = new ObservableCollection<Room>();
            Players = new ObservableCollection<string> { };

            is_loading = true;
            loadRoomsTask = LoadRoomsLoopAsync();

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
            Application.Current.Shutdown();
        }


        private async Task LoadRoomsLoopAsync()
        {
            while (is_loading)
            {
                await LoadRoomsAsync();
                await Task.Delay(3000);
            }
        }
        private async Task LoadRoomsAsync()
        {
            try
            {
                GetRoomsRequest req = new GetRoomsRequest();
                List<byte> buffer = Client.Instance.serializer.SerializeResponse(req);

                if (is_loading)
                {
                    ServerAnswer answer = await Task.Run(() => Client.Instance.communicator.SendAndReceive(buffer));

                    GetRoomsResponse response = JsonSerializer.Deserialize<GetRoomsResponse>(answer.json);

                    Rooms.Clear();
                    if (response != null && response.rooms != null)
                    {
                        foreach (RoomData room in response.rooms)
                        {
                            Rooms.Add(new Room(room.name, room.maxPlayers, room.timePerQuestion,room.id));
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to fetch rooms: " + ex.Message);
            }
        }
        private async void LoadRooms(object sender, RoutedEventArgs e)
        {
            await LoadRoomsAsync();
        }


        private async Task refreshMembersListLoopAsync()
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
                if(is_refreshing)
                {
                    ServerAnswer playerAnswer = Client.Instance.communicator.SendAndReceive(playerReqBuffer);

                    GetRoomStateResponse playerRes = JsonSerializer.Deserialize<GetRoomStateResponse>(playerAnswer.json);
                    if (playerRes.status == 2)
                    {
                        RoomGotClosed();
                    }
                    else if(playerRes.status == 1)
                    {
                        MessageBox.Show("Game Started!");
                        BackToMenu_Click(null,null);
                    }
                    else if (playerRes != null && playerRes.players != null)
                    {
                        Players.Clear();
                        foreach (var player in playerRes.players)
                        {
                            Players.Add(player);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error in refreshMembersList: " + ex.Message);
            }
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            Button btn = sender as Button;
            Room selectedRoom = btn.Tag as Room;

            if (selectedRoom != null)
            {
                try
                {
                    JoinRoomRequest req = new JoinRoomRequest { RoomId = (byte)selectedRoom.Id };
                    List<byte> buffer = Client.Instance.serializer.SerializeResponse(req);
                    ServerAnswer answer = Client.Instance.communicator.SendAndReceive(buffer);

                    if (answer.code == 0)
                    {
                        is_loading = false; //disable the room loading
                        RoomPlayerInfoPanel.Visibility = Visibility.Visible;
                        RoomNameShow.Text = "Room name: " + selectedRoom.Name;
                        PlayerAmountShow.Text = "Player amount: " + selectedRoom.PlayerCount;
                        TimeToAnswerShow.Text = "Time to answer (seconds): " + selectedRoom.TimeToAnswer;

                        GetRoomStateStruct getRoomStatereq = new GetRoomStateStruct();
                        List<byte> playerReqBuffer = Client.Instance.serializer.SerializeResponse(getRoomStatereq);
                        ServerAnswer playerAnswer = Client.Instance.communicator.SendAndReceive(playerReqBuffer);

                        GetRoomStateResponse playerRes = JsonSerializer.Deserialize<GetRoomStateResponse>(playerAnswer.json);
                        Players.Clear();
                        foreach (string player in playerRes.players)
                        {
                            Players.Add(player);
                        }
                        PlayerListShow.ItemsSource = Players;

                        is_refreshing = true;
                        refreshPlayerListTask = refreshMembersListLoopAsync();
                    }
                    else
                    {
                        ErrorMessage1.Visibility = Visibility.Visible;
                        ErrorMessage1.Text = "Failed to join room.";
                    }
                }
                catch (Exception ex)
                {
                    ErrorMessage1.Visibility = Visibility.Visible;
                    ErrorMessage1.Text = "Error joining room: " + ex.Message;
                }
            }
        }

        private void leaveRoom()
        {
            leaveRoomStruct leaveRoomReq = new leaveRoomStruct();
            List<byte> leaveRoomBuffer = Client.Instance.serializer.SerializeResponse(leaveRoomReq);
            ServerAnswer leaveRoomAnswer = Client.Instance.communicator.SendAndReceive(leaveRoomBuffer);
        }


        private void RoomGotClosed()
        {
            is_loading = false;
            is_refreshing = false;

            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Close();
        }
         

        private async void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            is_loading = false;

            if (is_refreshing)
            {
                is_refreshing = false;
                leaveRoom();
                if (refreshPlayerListTask != null) await refreshPlayerListTask;
            }
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Hide();
        }
    }
}