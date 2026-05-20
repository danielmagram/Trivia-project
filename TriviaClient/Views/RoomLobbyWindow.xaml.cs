using System;
using System.Windows;
using System.Windows.Threading;
using TriviaClient.Networking;
using TriviaClient.Models;
using TriviaClient.State;

namespace TriviaClient.Views
{
    public enum RoomCommand
    {
        CloseRoom = 151,
        StartGame = 152,
        GetRoomState = 153,
        LeaveRoom = 154,
        RoomClosedByAdmin = 155
    }

    public partial class RoomLobbyWindow : Window
    {
        private DispatcherTimer _pollingTimer;

        public RoomLobbyWindow()
        {
            InitializeComponent();
            UsernameText.Text = SessionData.Username;

            _pollingTimer = new DispatcherTimer();
            _pollingTimer.Interval = TimeSpan.FromSeconds(1.5);
            _pollingTimer.Tick += (s, e) => PollRoomState();

            if (SessionData.IsAdmin)
            {
                BtnStartGame.Visibility = Visibility.Visible;
                BtnCloseRoom.Visibility = Visibility.Visible;
            }
            else
            {
                BtnLeaveRoom.Visibility = Visibility.Visible;
            }

            PollRoomState();

            _pollingTimer.Start();
        }

        private void PollRoomState()
        {
            try
            {
                Communicator.Instance.SendRequest((byte)RoomCommand.GetRoomState, Serializer.Serialize(new GetRoomStateRequest()));

                ResponseInfo info = Communicator.Instance.ReceiveResponse();

                var response = Serializer.Deserialize<GetRoomStateResponse>(info.JsonPayload);
                if (response.HasGameBegun)
                {
                    _pollingTimer.Stop();
                    MessageBox.Show("The game is starting!", "Game Start", MessageBoxButton.OK, MessageBoxImage.Information);

                    // GameWindow gameWin = new GameWindow();
                    // gameWin.Show();
                    this.Close();
                }

                if (response.Players.Count == 0)
                {
                    HandleRoomClosedByAdmin("admin closed the room");

                    Communicator.Instance.SendRequest((byte)RoomCommand.RoomClosedByAdmin, Serializer.Serialize(new LeaveRoomRequest()));
                    ResponseInfo info2 = Communicator.Instance.ReceiveResponse();
                    var response2 = Serializer.Deserialize<CloseRoomResponse>(info2.JsonPayload);
                    if (response.Status != 5)
                    {
                        MessageBox.Show("Error notifying server of room closure.");
                    }

                    return;
                }
                if (response.Status == 5)
                {
                    HandleRoomClosedByAdmin("room not found");
                    return;
                }
                ListPlayers.ItemsSource = response.Players;

            }
            catch (Exception)
            {
                HandleRoomClosedByAdmin("Lost connection to the room server.");
            }
        }

        private void HandleRoomClosedByAdmin(string message)
        {
            _pollingTimer.Stop();
            MessageBox.Show(message, "Room Closed", MessageBoxButton.OK, MessageBoxImage.Warning);

            MenuWindow menu = new MenuWindow();
            menu.Show();
            this.Close();
        }

        private void LeaveButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                _pollingTimer.Stop();

                Communicator.Instance.SendRequest((byte)RoomCommand.LeaveRoom, Serializer.Serialize(new LeaveRoomRequest()));
                Communicator.Instance.ReceiveResponse(); // Await confirmation response

                MenuWindow window = new MenuWindow();
                window.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error leaving room: {ex.Message}");
                _pollingTimer.Start();
            }
        }

        private void CloseRoomButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                _pollingTimer.Stop();

                Communicator.Instance.SendRequest((byte)RoomCommand.CloseRoom, Serializer.Serialize(new CloseRoomRequest()));
                Communicator.Instance.ReceiveResponse();

                MenuWindow window = new MenuWindow();
                window.Show();
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error closing room: {ex.Message}");
                _pollingTimer.Start();
            }
        }

        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Communicator.Instance.SendRequest((byte)RoomCommand.StartGame, Serializer.Serialize(new StartGameRequest()));
                Communicator.Instance.ReceiveResponse(); // Await confirmation response
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error starting game: {ex.Message}");
            }
        }
    }
}
