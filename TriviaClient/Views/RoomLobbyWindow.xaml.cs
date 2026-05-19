using System;
using System.Windows;
using System.Windows.Threading;
using TriviaClient.Networking;
using TriviaClient.Models;
using TriviaClient.State;

namespace TriviaClient.Views
{
    public partial class RoomLobbyWindow : Window
    {
        private DispatcherTimer _pollingTimer;

        private const byte GET_ROOM_STATE_CODE = 153;
        private const byte CLOSE_ROOM_CODE = 151;
        private const byte START_GAME_CODE = 152;
        private const byte LEAVE_ROOM_CODE = 154;

        public RoomLobbyWindow()
        {   
            InitializeComponent();
            UsernameText.Text = SessionData.Username;

            _pollingTimer = new DispatcherTimer();
            _pollingTimer.Interval = TimeSpan.FromSeconds(1.5);
            _pollingTimer.Tick += (s, e) => PollRoomState();

            // 2. Set up button visibilities based on user role
            if (SessionData.IsAdmin)
            {
                BtnStartGame.Visibility = Visibility.Visible;
                BtnCloseRoom.Visibility = Visibility.Visible;
            }
            else
            {
                BtnLeaveRoom.Visibility = Visibility.Visible;
            }

            // 3. Query initial room state 
            PollRoomState();

            // 4. Start the timer loop if the initial poll didn't crash/exit
            _pollingTimer.Start();
        }

        private void PollRoomState()
        {
            try
            {
                // Send GET_ROOM_STATE request to the server
                Communicator.Instance.SendRequest(GET_ROOM_STATE_CODE, Serializer.Serialize(new GetRoomStateRequest()));

                ResponseInfo info = Communicator.Instance.ReceiveResponse();

                if (info.Code == 5) 
                {
                    HandleRoomClosedByAdmin("room not found");
                    return;
                }

                var response = Serializer.Deserialize<GetRoomStateResponse>(info.JsonPayload);

                // Update the visual player list
                if (response.Players != null)
                {
                    ListPlayers.ItemsSource = response.Players;
                }

                // If the admin started the game, transition immediately!
                if (response.HasGameBegun)
                {
                    _pollingTimer.Stop();
                    MessageBox.Show("The game is starting!", "Game Start", MessageBoxButton.OK, MessageBoxImage.Information);

                    // GameWindow gameWin = new GameWindow();
                    // gameWin.Show();
                    this.Close();
                }
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
                Communicator.Instance.SendRequest(LEAVE_ROOM_CODE, Serializer.Serialize(new LeaveRoomRequest()));
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
                Communicator.Instance.SendRequest(CLOSE_ROOM_CODE, Serializer.Serialize(new CloseRoomRequest()));
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
                Communicator.Instance.SendRequest(START_GAME_CODE, Serializer.Serialize(new StartGameRequest()));
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error starting game: {ex.Message}");
            }
        }
    }
}