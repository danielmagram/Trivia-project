using System;
using System.Collections.Generic;
using System.Windows;

namespace TriviaClient.Views
{
    // Helper class to represent a row in the results table
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

            // Load temporary mock data so you can see the styled table
            LoadMockResults();
        }

        private void LoadMockResults()
        {
            // Fictional results list for visual testing
            List<PlayerResultData> results = new List<PlayerResultData>
            {
                new PlayerResultData { PlayerName = "Alice", CorrectAnswers = 4, AverageTime = 2.4 },
                new PlayerResultData { PlayerName = "Bob", CorrectAnswers = 5, AverageTime = 1.8 },
                new PlayerResultData { PlayerName = "Charlie", CorrectAnswers = 2, AverageTime = 4.1 }
            };

            // Display the data in the table
            LvResults.ItemsSource = results;

            // Logic to find the winner (if the server doesn't return a pre-sorted list)
            PlayerResultData winner = FindWinner(results);

            if (winner != null)
            {
                TxtWinnerName.Text = winner.PlayerName;
            }
        }

        // Function that calculates who won (Most correct answers, tie-breaker: fastest average time)
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
                    // Tie in correct answers - the one who answered faster wins
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
            // Open the Main Menu window and close this one
            MenuWindow menu = new MenuWindow();
            menu.Show();
            this.Close();
        }
    }
}