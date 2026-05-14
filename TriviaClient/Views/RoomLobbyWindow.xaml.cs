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
using TriviaClient.Networking;
using TriviaClient.Models;
using TriviaClient.State;

namespace TriviaClient.Views
{
    /// <summary>
    /// Interaction logic for RoomLobbyWindow.xaml
    /// </summary>
    public partial class RoomLobbyWindow : Window
    {
        public RoomLobbyWindow()
        {
            InitializeComponent();
            UpdateRoom( (uint)SessionData.RoomId);
        }
        private void UpdateRoom(uint roomId)
        {
            try
            {
                Communicator.Instance.SendRequest(105, Serializer.Serialize(new GetPlayersInRoomRequest { roomId = roomId }));

                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<GetPlayersInRoomResponse>(info.JsonPayload);

                if(response.Players.Count != 0)
                {
                    ListPlayers.ItemsSource = response.Players;
                }

                
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to fetch players in room: {ex.Message}", "Network Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            UpdateRoom((uint)SessionData.RoomId);
            MessageBox.Show("Player list updated!");
        }

        private void LeaveButton_Click(object sender, RoutedEventArgs e)
        {
            JoinRoomWindow window = new JoinRoomWindow();
            window.Show();

            this.Close();
        }
    }
}
