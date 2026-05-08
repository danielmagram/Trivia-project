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
using TriviaClient.Models;
using TriviaClient.Networking;

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

            SignupRequest req = new SignupRequest();
            req.Username = username;
            req.Password = password;    
            req.Email = email;

            Communicator.Instance.SendRequest(33, Serializer.Serialize(req));
            ResponseInfo info = Communicator.Instance.ReceiveResponse();
            SignupResponse response = new SignupResponse();
            response = Serializer.Deserialize<SignupResponse>(info.JsonPayload);
            if (response.Status == 1)
            {
                MenuWindow start = new MenuWindow();
                start.Show();
                this.Close();
            }
            MessageBox.Show("Signup failed!");
        }
    }
}
