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
using TriviaClient.Models;
using TriviaClient.Networking;
using TriviaClient.State;

namespace TriviaClient.Views
{
    /// <summary>
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        public CreateRoomWindow()
        {
            InitializeComponent();
            UsernameText.Text = SessionData.Username;
        }
        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            MenuWindow menu = new MenuWindow();
            menu.Show();
            this.Close();
        }
        private void SendButton_Click(object sender, RoutedEventArgs e)
        {
            string name = RoomNameInput.Text;
            string playersStr = PlayersInput.Text;
            string timeStr = TimeInput.Text;
            string questionStr = QuestionsInput.Text;

            if (string.IsNullOrWhiteSpace(name) || string.IsNullOrWhiteSpace(playersStr) || string.IsNullOrWhiteSpace(timeStr) || string.IsNullOrWhiteSpace(questionStr))
            {
                MessageBox.Show("Please fill in all fields.", "Input Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            if (!uint.TryParse(questionStr, out uint questions) || !uint.TryParse(timeStr, out uint time) || !uint.TryParse(playersStr, out uint players))
            {
                MessageBox.Show("Players, Time, and Questions must be valid numbers.", "Input Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                CreateRoomRequest req = new CreateRoomRequest
                {
                    RoomName = name,
                    MaxUsers = players,
                    AnswerTimeout = time,
                    QuestionCount = questions
                };

                Communicator.Instance.SendRequest(101, Serializer.Serialize(req));

                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<CreateRoomResponse>(info.JsonPayload);


                switch (response.Status)
                {
                    case 1: // SUCCESS
                        MessageBox.Show("Room created successfully!", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
                        MenuWindow menu = new MenuWindow();
                        menu.Show();
                        this.Close();
                        break;

                    default:
                        MessageBox.Show($"Unexpected status code: {response.Status}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Network Error: {ex.Message}", "Connection Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}