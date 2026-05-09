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
            UsernameText.Text = "        User123";
        }
        private void Logout_Click(object sender, RoutedEventArgs e)
        {
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

        private void MyStatus_Click(object sender, RoutedEventArgs e)
        {
            StatusWindow status = new StatusWindow();
            status.Show();

            this.Close();
        }

        private void BestScores_Click(object sender, RoutedEventArgs e)
        {
            BestScoresWindow scores = new BestScoresWindow();
            scores.Show();

            this.Close();
        }

        private void Quit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

    }
}
