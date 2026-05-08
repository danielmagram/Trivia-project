using System.Windows;
using System.Windows.Controls;

namespace TriviaClient.Views
{
    public partial class SignupWindow : Window
    {
        public SignupWindow()
        {
            InitializeComponent();
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            StartupWindow start = new StartupWindow();
            start.Show();
            this.Close();
        }

        private void Send_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameBox.Text;
            string password = PasswordBox.Password;
            string email = EmailBox.Text;

        }
    }
}
