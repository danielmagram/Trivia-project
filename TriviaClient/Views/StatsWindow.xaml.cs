using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Shapes;
using TriviaClient.Models;
using TriviaClient.Networking;

namespace TriviaClient.Views
{
    /// <summary>
    /// Interaction logic for StatsWindow.xaml
    /// </summary>
    public partial class StatsWindow : Window
    {
        public StatsWindow()
        {
            InitializeComponent();
            GetPersonalStatsRequest request = new GetPersonalStatsRequest();
            Communicator.Instance.SendRequest(150, Serializer.Serialize(request));
            ResponseInfo info = Communicator.Instance.ReceiveResponse();
            var response = Serializer.Deserialize<GetPersonalStatsResponse>(info.JsonPayload);
            GamesCountText.Text = $"number of games: {response.Statistics[0]}";
            TotalAnswersText.Text = $"number of total answers: {response.Statistics[2]}";
            RightAnswersText.Text = $"number of right answers: {response.Statistics[2]}";
            WrongAnswersText.Text = $"number of wrong answers: {response.Statistics[3]}";
            AvgTimeText.Text = $"average time for answer: {response.Statistics[4]}s";
        }
        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            GetPersonalStatsRequest request = new GetPersonalStatsRequest();
            Communicator.Instance.SendRequest(150, Serializer.Serialize(request));
            ResponseInfo info = Communicator.Instance.ReceiveResponse();
            var response = Serializer.Deserialize<GetPersonalStatsResponse>(info.JsonPayload);
            GamesCountText.Text = $"number of games: {response.Statistics[0]}";
            RightAnswersText.Text = $"number of right answers: {response.Statistics[1]}";
            WrongAnswersText.Text = $"number of wrong answers: {response.Statistics[2]}";
            AvgTimeText.Text = $"average time for answer: {response.Statistics[3]}s";
        }
    }
}
