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
            // 1. Validation: Don't even talk to the server if fields are empty
            if (string.IsNullOrWhiteSpace(txtUsername.Text) || string.IsNullOrWhiteSpace(txtPassword.Password))
            {
                MessageBox.Show("Please enter both username and password.", "Input Error", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                // 2. Prepare the Request
                var loginReq = new LoginRequest
                {
                    Username = txtUsername.Text,
                    Password = txtPassword.Password
                };

                // 3. Serialize to JSON
                string jsonPayload = Serializer.Serialize(loginReq);

                // 4. Send to Server (Code 200 for Login)
                Communicator.Instance.SendRequest(200, jsonPayload);

                // 5. Wait for Response
                ResponseInfo response = Communicator.Instance.ReceiveResponse();

                // 6. Handle the result
                var loginRes = Serializer.Deserialize<LoginResponse>(response.JsonPayload);

                if (loginRes.Status == 1) // Assuming 1 = Success in your C++ logic
                {
                    // Success! Save user info to the global state
                    SessionData.Username = txtUsername.Text;

                    // Navigate to Menu (Assuming your partner created MenuWindow)
                    //MenuWindow menu = new MenuWindow();
                    //menu.Show();
                    //this.Close();
                }
                else
                {
                    MessageBox.Show("Invalid username or password.", "Login Failed", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Network Error: {ex.Message}", "Connection Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            // If your partner's StartupWindow is ready, show it and close this one
            var startup = new StartupWindow();
            startup.Show();
            this.Close();
        }
    }
}