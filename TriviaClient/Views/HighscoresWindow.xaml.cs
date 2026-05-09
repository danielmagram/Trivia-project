using System;
using System.Collections.Generic;
using System.Windows;
using TriviaClient.Models;
using TriviaClient.Networking;

namespace TriviaClient.Views
{
    public partial class HighscoresWindow : Window
    {
        public HighscoresWindow()
        {
            InitializeComponent();
            LoadHighscores(); // Fetch data as soon as window is created
        }

        private void LoadHighscores()
        {
            try
            {
                var req = new GetHighscoresRequest();
                string json = Serializer.Serialize(req);

                Communicator.Instance.SendRequest(140, json);

                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<GetHighScoreResponse>(info.JsonPayload);

                if (response.Status == 1)
                {
                    
                    lstHighscores.ItemsSource = response.Statistics;
                }
                else
                {
                    MessageBox.Show("Failed to load highscores.", "Error", MessageBoxButton.OK, MessageBoxImage.Warning);
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