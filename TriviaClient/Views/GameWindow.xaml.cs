using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using TriviaClient.Networking;
using TriviaClient.Models;

namespace TriviaClient.Views
{
    public partial class GameWindow : Window
    {
        private DispatcherTimer _questionTimer;
        private int _timeLeft;
        private int _correctAnswersCount = 0;
        private int _questionsRemaining = 5; // will be set based on server response when loading the first question

        // will help us map each answer button to its corresponding answer ID (0-3)
        private Dictionary<Button, int> _answerButtonMap;

        public GameWindow()
        {
            InitializeComponent();
            InitTimer();
            InitButtonMap();

            // Loading the first question (currently using Mock, will replace with server call when partner finishes)
            LoadMockQuestion();
        }

        private void InitTimer()
        {
            _questionTimer = new DispatcherTimer();
            _questionTimer.Interval = TimeSpan.FromSeconds(1);
            _questionTimer.Tick += Timer_Tick;
        }

        private void InitButtonMap()
        {
            _answerButtonMap = new Dictionary<Button, int>
            {
                { BtnAnswer0, 0 },
                { BtnAnswer1, 1 },
                { BtnAnswer2, 2 },
                { BtnAnswer3, 3 }
            };
        }

        private void StartNewQuestionTimer(int seconds)
        {
            _timeLeft = seconds;
            TxtTimer.Text = $"{_timeLeft} seconds";
            _questionTimer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            _timeLeft--;
            TxtTimer.Text = $"{_timeLeft} seconds";

            if (_timeLeft <= 0)
            {
                _questionTimer.Stop();
                MessageBox.Show("Time's up for this question!", "Time's Up", MessageBoxButton.OK, MessageBoxImage.Warning);
                HandleAnswerSubmission(4);
            }
        }

        private void LoadMockQuestion()
        {
            // Update the counters on the screen
            TxtQuestionsRemaining.Text = _questionsRemaining.ToString();
            TxtCorrectAnswers.Text = _correctAnswersCount.ToString();

            // Temporary data for visual testing only
            TxtQuestion.Text = "whats the capital of France?";
            BtnAnswer0.Content = "London";
            BtnAnswer1.Content = "Paris";
            BtnAnswer2.Content = "Rome";
            BtnAnswer3.Content = "Berlin";

            StartNewQuestionTimer(10); // Timer for 10 seconds as an example
        }

        private void AnswerButton_Click(object sender, RoutedEventArgs e)
        {
            _questionTimer.Stop();
            Button clickedButton = sender as Button;
            int selectedAnswerId = _answerButtonMap[clickedButton];

            // Local temporary check (if clicked on Paris - answer 1)
            if (selectedAnswerId == 1)
            {
                _correctAnswersCount++;
                MessageBox.Show("Correct answer!", "Well Done", MessageBoxButton.OK, MessageBoxImage.Information);
            }
            else
            {
                MessageBox.Show("Wrong answer!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }

            _questionsRemaining--;

            if (_questionsRemaining > 0)
            {
                LoadMockQuestion(); // Load the next question
            }
            else
            {
                // 
                GameResultsWindow resultsWindow = new GameResultsWindow();
                resultsWindow.Show();
                this.Close();
            }
        }

        private void HandleAnswerSubmission(int answerId)
        {
            // will handle sending the answer to the server and processing the response when we integrate with the backend

            _questionsRemaining--;
            if (_questionsRemaining > 0) LoadMockQuestion();
            else
            {
                GameResultsWindow resultsWindow = new GameResultsWindow();
                resultsWindow.Show();
                this.Close();
            }
        }
    }
}