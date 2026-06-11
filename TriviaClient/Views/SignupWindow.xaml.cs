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
            string address = AddressBox.Text;
            string phoneNumber = PhoneNumberBox.Text;
            string dateOfBirth = DateOfBirthBox.Text;

            if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password) || string.IsNullOrWhiteSpace(email) || string.IsNullOrWhiteSpace(address) || string.IsNullOrWhiteSpace(phoneNumber) || string.IsNullOrWhiteSpace(dateOfBirth))
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
                    Email = email,
                    Address = address,
                    PhoneNumber = phoneNumber,
                    DateOfBirth = dateOfBirth
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
                    case 12: // INVALID PASSWORD
                        MessageBox.Show("Password is not valid. a valid password need to have at least 8 characters, including uppercase, lowercase, number and special character.", "Signup Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;
                    case 13: // INVALID EMAIL
                        MessageBox.Show("Email is not valid. Please enter a valid email address.", "Signup Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;
                    case 14: // INVALID ADDRESS
                        MessageBox.Show("Address is not valid. A valid address should be in the format 'Street, Number, City'.", "Signup Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;
                    case 15:// INVALID PHONE NUMBER
                        MessageBox.Show("Phone number is not valid. A valid phone number should be in the format '0XX-XXXXXXX or 0X-XXXXXXX'.", "Signup Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
                        break;
                    case 16: // INVALID DATE OF BIRTH
                        MessageBox.Show("Date of birth is not valid. A valid date of birth should be in the format DD.MM.YYYY OR DD/MM/YYYY.", "Signup Failed", MessageBoxButton.OK, MessageBoxImage.Warning);
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
