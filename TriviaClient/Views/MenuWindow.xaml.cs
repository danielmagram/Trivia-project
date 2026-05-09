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
using TriviaClient.State;

namespace TriviaClient.Views
{
    /// <summary>
    /// Interaction logic for MenuWindow.xaml
    /// </summary>
    public partial class MenuWindow : Window
    {
        public MenuWindow()
        {
            InitializeComponent();
            UsernameText.Text = "        " + SessionData.Username;
        }
        private void Logout_Click(object sender, RoutedEventArgs e)
        {
            Communicator.Instance.Disconnect();
            SessionData.Clear();
            StartupWindow startup = new StartupWindow();
            startup.Show();
            this.Close();
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoomWindow joinRoom = new JoinRoomWindow();
            joinRoom.Show();

            this.Close();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoomWindow createRoom = new CreateRoomWindow();
            createRoom.Show();

            this.Close();
        }

        private void MyStats_Click(object sender, RoutedEventArgs e)
        {
            StatsWindow stats = new StatsWindow();
            stats.Show();

            this.Close();
        }

        private void BestScores_Click(object sender, RoutedEventArgs e)
        {
            HighscoresWindow scores = new HighscoresWindow();
            scores.Show();

            this.Close();
        }

        private void Quit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

    }
}
