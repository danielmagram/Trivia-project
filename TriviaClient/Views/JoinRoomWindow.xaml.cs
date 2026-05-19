using System;
using System.Collections.Generic;
using System.Windows;
using TriviaClient.Models;
using TriviaClient.Networking;
using TriviaClient.State;
namespace TriviaClient.Views
{
    public partial class JoinRoomWindow : Window
    {
        public JoinRoomWindow()
        {
            InitializeComponent();
            LoadRooms();
        }

        private void LoadRooms()
        {
            try
            {
                Communicator.Instance.SendRequest(102, Serializer.Serialize(new GetRoomsRequest()));

                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<GetRoomsResponse>(info.JsonPayload);

                if (response.Rooms.Count != 0)
                {
                    lvRooms.ItemsSource = response.Rooms;
                }
                else
                {
                    lvRooms.ItemsSource = new List<RoomData>(); // Clear list if empty
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to fetch rooms: {ex.Message}", "Network Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void btnRefresh_Click(object sender, RoutedEventArgs e)
        {
            LoadRooms();
        }

        private void btnJoin_Click(object sender, RoutedEventArgs e)
        {
            // Verify a room is actually selected
            if (lvRooms.SelectedItem is not RoomData selectedRoom)
            {
                MessageBox.Show("Please select a room from the list first.", "No Room Selected", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                var joinReq = new JoinRoomRequest { roomId = selectedRoom.Id };

                Communicator.Instance.SendRequest(100, Serializer.Serialize(joinReq));

                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<JoinRoomResponse>(info.JsonPayload);

                // Handle server status responses
                switch (response.Status)
                {
                   case 1: // SUCCESS
                        MessageBox.Show("Successfully joined the room!", "Join Successful", MessageBoxButton.OK, MessageBoxImage.Information);

                        SessionData.IsAdmin = false; 
                        SessionData.RoomId = (int)selectedRoom.Id;

                        RoomLobbyWindow lobby = new RoomLobbyWindow();
                        lobby.Show();
                        this.Close();
                        break;

                    case 4: 
                        MessageBox.Show("This room is already full. Try joining another one.", "Room Full", MessageBoxButton.OK, MessageBoxImage.Information);
                        LoadRooms(); 
                        break;

                    case 5: 
                        MessageBox.Show("This room no longer exists. It may have been closed.", "Room Not Found", MessageBoxButton.OK, MessageBoxImage.Warning);
                        LoadRooms();
                        break;

                    default:
                        MessageBox.Show($"Could not join room. Error code: {response.Status}", "Join Failed", MessageBoxButton.OK, MessageBoxImage.Error);
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Network Error: {ex.Message}", "Connection Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            MenuWindow menu = new MenuWindow();
            menu.Show();
            this.Close();
        }
    }
}