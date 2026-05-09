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

            if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password) || string.IsNullOrWhiteSpace(email))
            {
                MessageBox.Show("Please fill in all fields.", "Input Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                SignupRequest req = new SignupRequest
                {
                    Username = username,
                    Password = password,
                    Email = email
                };

                Communicator.Instance.SendRequest(33, Serializer.Serialize(req));

                ResponseInfo info = Communicator.Instance.ReceiveResponse();
                var response = Serializer.Deserialize<SignupResponse>(info.JsonPayload);


                switch (response.Status)
                {
                    case 1: // SUCCESS
                        MessageBox.Show("Account created successfully!", "Success", MessageBoxButton.OK, MessageBoxImage.Information);
                        SessionData.Username = username;
                        MenuWindow menu = new MenuWindow();
                        menu.Show();
                        this.Close();
                        break;

                    case 7: // EMPTY VALUE
                        MessageBox.Show("One or more fields were empty.", "Signup Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;

                    case 8: // USERNAME EXISTS
                        MessageBox.Show("This username is already taken. Please choose another.", "Signup Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;

                    case 2: // DB ERROR
                        MessageBox.Show("A database error occurred on the server.", "Server Error", MessageBoxButton.OK, MessageBoxImage.Error);
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
