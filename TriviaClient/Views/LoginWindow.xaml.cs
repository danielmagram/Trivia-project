using System;
using System.Windows;
using TriviaClient.Networking;
using TriviaClient.Models;
using TriviaClient.State;

namespace TriviaClient.Views
{
    public partial class LoginWindow : Window
    {
        public LoginWindow()
        {
            InitializeComponent();
        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(txtUsername.Text) || string.IsNullOrWhiteSpace(txtPassword.Password))
            {
                MessageBox.Show("Please enter both username and password.", "Input Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                var loginReq = new LoginRequest
                {
                    Username = txtUsername.Text,
                    Password = txtPassword.Password
                };

                string jsonPayload = Serializer.Serialize(loginReq);

                Communicator.Instance.SendRequest(202, jsonPayload);

                ResponseInfo response = Communicator.Instance.ReceiveResponse();


                var loginRes = Serializer.Deserialize<LoginResponse>(response.JsonPayload);

                switch (loginRes.Status)
                {
                    case 1: // SUCCESS
                        SessionData.Username = txtUsername.Text;
                        MainWindow menu = new MainWindow();
                        menu.Show();
                        this.Close();
                        break;

                    case 3: // WRONG PARAMS
                        MessageBox.Show("Invalid username or password.", "Login Failed", MessageBoxButton.OK, MessageBoxImage.Error);
                        break;

                    case 9: // ALREADY LOGGED IN
                        MessageBox.Show("This user is already logged in from another location.", "Login Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;

                    case 2: // DB ERROR
                        MessageBox.Show("A database error occurred on the server. Please try again later.", "Server Error", MessageBoxButton.OK, MessageBoxImage.Stop);
                        break;

                    default: // FALLBACK
                        MessageBox.Show($"Unknown error occurred. Status Code: {loginRes.Status}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Network Error: {ex.Message}", "Connection Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            var startup = new StartupWindow();
            startup.Show();
            this.Close();
        }
    }
}