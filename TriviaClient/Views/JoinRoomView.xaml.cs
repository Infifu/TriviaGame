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
using TriviaClient.ViewModel;
using WPFTEST.Services;

public class Room
{
    public byte Id { get; set; }
    public string Name { get; set; }
    public int PlayerCount { get; set; }
    public int TimeToAnswer { get; set; }
    public string PlayerStatus => $"Players: {PlayerCount}";

    public Room(string name, int playerCount, int timeToAnswer, byte id = 0)
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

            Rooms = new ObservableCollection<Room>();
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

        private void LoadRooms(object sender, RoutedEventArgs e)
        {
            try
            {
                GetRoomsRequest req = new GetRoomsRequest();
                List<byte> buffer = Client.Instance.serializer.SerializeResponse(req);
                ServerAnswer answer = Client.Instance.communicator.SendAndReceive(buffer);

                GetRoomsResponse response = JsonSerializer.Deserialize<GetRoomsResponse>(answer.json);

                foreach (RoomData room in response.rooms)
                {
                    Rooms.Add(new Room(room.name, room.maxPlayers, room.timePerQuestion));
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Failed to fetch rooms: " + ex.Message);
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
                    JoinRoomRequest req = new JoinRoomRequest { RoomId = selectedRoom.Id };
                    List<byte> buffer = Client.Instance.serializer.SerializeResponse(req);
                    ServerAnswer answer = Client.Instance.communicator.SendAndReceive(buffer);

                    if (answer.code == 0)
                    {
                        RoomPlayerInfoPanel.Visibility = Visibility.Visible;
                        RoomNameShow.Text = "Room name: " + selectedRoom.Name;
                        PlayerAmountShow.Text = "Player amount: " + selectedRoom.PlayerCount;
                        TimeToAnswerShow.Text = "Time to answer (seconds): " + selectedRoom.TimeToAnswer;

                        GetPlayersInRoomRequest playerReq = new GetPlayersInRoomRequest { roomId = selectedRoom.Id };
                        List<byte> playerReqBuffer = Client.Instance.serializer.SerializeResponse(playerReq);
                        ServerAnswer playerAnswer = Client.Instance.communicator.SendAndReceive(playerReqBuffer);

                        GetPlayersInRoomResponse playerRes = JsonSerializer.Deserialize<GetPlayersInRoomResponse>(playerAnswer.json);
                        if (playerRes != null && playerRes.players != null)
                        {
                            PlayerListShow.ItemsSource = playerRes.players;
                        }
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

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            MainMenu mainMenu = new MainMenu();
            mainMenu.Show();
            this.Hide();
        }
    }
}