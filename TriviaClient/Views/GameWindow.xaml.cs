using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using TriviaClient.Models;
using TriviaClient.Networking;
using TriviaClient.State;

namespace TriviaClient.Views
{
    public enum GameCommand
    {
        LeaveGame = 160,
        GetQuestion = 161,
        SubmitAnswer = 162,
        GetGameResults = 163
    }

    public partial class GameWindow : Window
    {
        private DispatcherTimer _questionTimer;
        private int _timeLeft;
        private int _correctAnswersCount = 0;
        private int _questionsRemaining = SessionData.QuestionCount;
        private DispatcherTimer _resultsPollingTimer;

        private Dictionary<Button, int> _answerButtonMap;

        public GameWindow()
        {
            InitializeComponent();
            InitTimer(); 

            _answerButtonMap = new Dictionary<Button, int>();

            LoadNextQuestion();
        }

        private void InitTimer()
        {
            _questionTimer = new DispatcherTimer();
            _questionTimer.Interval = TimeSpan.FromSeconds(1);
            _questionTimer.Tick += Timer_Tick;
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

        private void LoadNextQuestion()
        {
            try
            {
                Communicator.Instance.SendRequest((byte)GameCommand.GetQuestion, Serializer.Serialize(new GetQuestionRequest()));
                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<GetQuestionResponse>(info.JsonPayload);

                if (response.Status != 1) 
                {
                    MessageBox.Show("Failed to load question from server.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                TxtQuestionsRemaining.Text = _questionsRemaining.ToString();
                TxtCorrectAnswers.Text = _correctAnswersCount.ToString();
                TxtQuestion.Text = response.Question;

                Button[] buttons = { BtnAnswer0, BtnAnswer1, BtnAnswer2, BtnAnswer3 };
                _answerButtonMap.Clear();

                int i = 0;
                foreach (var answerArr in response.Answers)
                {
                    // Ensure the array has both the ID and the text string safely
                    if (i < 4 && answerArr != null && answerArr.Length >= 2)
                    {
                        uint answerId = Convert.ToUInt32(answerArr[0]);
                        string answerText = answerArr[1]?.ToString();

                        buttons[i].Content = answerText;

                        _answerButtonMap[buttons[i]] = (int)answerId;
                        i++;
                    }
                }

                StartNewQuestionTimer(SessionData.AnswerTimeout);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Lost connection to server: {ex.Message}");
            }
        }

        private void AnswerButton_Click(object sender, RoutedEventArgs e)
        {
            _questionTimer.Stop();

            Button clickedButton = sender as Button;
            int selectedAnswerId = _answerButtonMap[clickedButton];

            HandleAnswerSubmission(selectedAnswerId);
        }

        private void HandleAnswerSubmission(int answerId)
        {
            try
            {
                var req = new SubmitAnswerRequest { AnswerId = (uint)answerId };
                Communicator.Instance.SendRequest((byte)GameCommand.SubmitAnswer, Serializer.Serialize(req));
                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<SubmitAnswerResponse>(info.JsonPayload);

                if (response.CorrectAnswerId == answerId)
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
                    LoadNextQuestion();
                }
                else
                {
                    EnterWaitingForPlayersState();

                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error submitting answer: {ex.Message}");
            }
        }
        private void EnterWaitingForPlayersState()
        {
            TxtQuestion.Text = "Waiting for other players to finish...";
            BtnAnswer0.Visibility = Visibility.Collapsed;
            BtnAnswer1.Visibility = Visibility.Collapsed;
            BtnAnswer2.Visibility = Visibility.Collapsed;
            BtnAnswer3.Visibility = Visibility.Collapsed;
            TxtTimer.Text = "--";

            _resultsPollingTimer = new DispatcherTimer();
        _resultsPollingTimer.Interval = TimeSpan.FromSeconds(1.5);
            _resultsPollingTimer.Tick += (s, e) => PollForGameEnd();
        _resultsPollingTimer.Start();
        }

        private void PollForGameEnd()
        {
            try
            {
                _resultsPollingTimer.Stop(); 

                Communicator.Instance.SendRequest((byte)GameCommand.GetGameResults, Serializer.Serialize(new GetGameResultsRequest()));
                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<GetGameResultsResponse>(info.JsonPayload);

 
                if (response.Status == 1)
                {
                    GameResultsWindow resultsWindow = new GameResultsWindow();
                    resultsWindow.Show();
                    this.Close();
                }
                else if (response.Status == 11)
                {
                    _resultsPollingTimer.Start();
                }
            }
            catch (Exception)
            {
                _resultsPollingTimer.Start();
            }
        }
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            try
            {
                _questionTimer?.Stop();
                Communicator.Instance.SendRequest((byte)GameCommand.LeaveGame, Serializer.Serialize(new LeaveGameRequest()));
                Communicator.Instance.ReceiveResponse();
            }
            catch { }

            base.OnClosing(e);
        }
    }
}