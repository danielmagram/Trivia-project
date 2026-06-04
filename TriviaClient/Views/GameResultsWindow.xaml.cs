using System;
using System.Collections.Generic;
using System.Windows;
using TriviaClient.Networking;
using TriviaClient.Models;

namespace TriviaClient.Views
{
    public class PlayerResultData
    {
        public string PlayerName { get; set; }
        public int CorrectAnswers { get; set; }
        public double AverageTime { get; set; }
    }

    public partial class GameResultsWindow : Window
    {
        public GameResultsWindow()
        {
            InitializeComponent();

            LoadRealResults();
        }

        private void LoadRealResults()
        {
            try
            {
                Communicator.Instance.SendRequest((byte)GameCommand.GetGameResults, Serializer.Serialize(new GetGameResultsRequest()));
                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<GetGameResultsResponse>(info.JsonPayload);

                if (response.Status != 1)
                {
                    MessageBox.Show("Failed to fetch game results from the server.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                List<PlayerResultData> uiResults = new List<PlayerResultData>();
                foreach (var serverPlayer in response.Results)
                {
                    uiResults.Add(new PlayerResultData
                    {
                        PlayerName = serverPlayer.Username,
                        CorrectAnswers = (int)serverPlayer.CorrectAnswerCount,
                        AverageTime = Math.Round(serverPlayer.AverageAnswerTime, 2) 
                    });
                }

                LvResults.ItemsSource = uiResults;

                PlayerResultData winner = FindWinner(uiResults);

                if (winner != null)
                {
                    TxtWinnerName.Text = winner.PlayerName;
                }
                else
                {
                    TxtWinnerName.Text = "No clear winner";
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading leaderboard: {ex.Message}", "Network Failure", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private PlayerResultData FindWinner(List<PlayerResultData> results)
        {
            if (results == null || results.Count == 0) return null;

            PlayerResultData bestPlayer = results[0];

            foreach (var player in results)
            {
                if (player.CorrectAnswers > bestPlayer.CorrectAnswers)
                {
                    bestPlayer = player;
                }
                else if (player.CorrectAnswers == bestPlayer.CorrectAnswers)
                {
                    if (player.AverageTime < bestPlayer.AverageTime)
                    {
                        bestPlayer = player;
                    }
                }
            }

            return bestPlayer;
        }

        private void BtnMainMenu_Click(object sender, RoutedEventArgs e)
        {
            try
            {
               
                Communicator.Instance.SendRequest((byte)GameCommand.LeaveGame, "{}");
                var response=Communicator.Instance.ReceiveResponse();
            }
            catch { /* Ignore network errors if we are just disconnecting */ }

            MenuWindow menu = new MenuWindow();
            menu.Show();
            this.Close();
        }
    }
}