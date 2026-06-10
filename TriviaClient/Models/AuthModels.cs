
using Newtonsoft.Json;

namespace TriviaClient.Models
{
    //SHARED DATA STRUCTURES
    public class RoomData
    {
        [JsonProperty("id")]
        public uint Id { get; set; }

        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("maxPlayers")]
        public uint MaxPlayers { get; set; }

        [JsonProperty("timePerQuestion")]
        public uint TimePerQuestion { get; set; }

        [JsonProperty("isActive")]
        public uint IsActive { get; set; }
    }

    //REQUESTS 

    public class LoginRequest
    {
        [JsonProperty("username")]
        public string Username { get; set; }

        [JsonProperty("password")]
        public string Password { get; set; }
    }

    public class SignupRequest
    {
        [JsonProperty("username")]
        public string Username { get; set; }

        [JsonProperty("password")]
        public string Password { get; set; }

        [JsonProperty("email")]
        public string Email { get; set; }

        [JsonProperty("address")]
        public string Address { get; set; }

        [JsonProperty("phoneNumber")]
        public string PhoneNumber { get; set; }

        [JsonProperty("dateOfBirth")]
        public string DateOfBirth { get; set; }
    }

    public class GetPlayersInRoomRequest // fix
    {
        [JsonProperty("roomId")]
        public uint roomId { get; set; }
    }

    public class JoinRoomRequest
    {
        [JsonProperty("roomId")]
        public uint roomId { get; set; }
    }

    public class CreateRoomRequest
    {
        [JsonProperty("roomName")]
        public string RoomName { get; set; }

        [JsonProperty("maxUsers")]
        public uint MaxUsers { get; set; }

        [JsonProperty("questionCount")]
        public uint QuestionCount { get; set; }

        [JsonProperty("answerTimeout")]
        public uint AnswerTimeout { get; set; }
    }
    public class GetHighscoresRequest
    {
        
    }

    public class GetPersonalStatsRequest
    {

    }

    // RESPONSES 
    public class ErrorResponse
    {
        [JsonProperty("message")]
        public string Message { get; set; }
    }

    public class LoginResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class SignupResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class LogoutResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class GetRoomsResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("rooms")]
        public List<RoomData> Rooms { get; set; }
    }

    public class GetPlayersInRoomResponse //fix
    {
        [JsonProperty("PlayersInRoom")]
        public List<string> Players { get; set; }
    }

    public class JoinRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class CreateRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class GetHighScoreResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("HighScores")]
        public List<string> Statistics { get; set; }
    }

    public class GetPersonalStatsResponse
    {
        [JsonProperty("Status")]
        public uint Status { get; set; }

        [JsonProperty("UserStatistics")]
        public List<string> Statistics { get; set; }
    }
    public class GetRoomsRequest { }


    public class GetRoomStateRequest
    {
    }

    public class GetRoomStateResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("hasGameBegun")]
        public bool HasGameBegun { get; set; }

        [JsonProperty("players")]
        public List<string> Players { get; set; }

        [JsonProperty("questionCount")]
        public uint QuestionCount { get; set; }

        [JsonProperty("answerTimeout")]
        public float AnswerTimeout { get; set; }
    }

    public class CloseRoomRequest
    {
    }
    public class CloseRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }
    public class StartGameRequest
    {
    }

    public class StartGameResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class LeaveRoomRequest
    {
    }

    public class LeaveRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    //GAME REQUESTS
    public class GetQuestionRequest { }

    public class SubmitAnswerRequest
    {
        [JsonProperty("answerId")]
        public uint AnswerId { get; set; }
    }

    public class GetGameResultsRequest { }

    public class LeaveGameRequest { }

    //GAME RESPONSES 
    public class GetQuestionResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("question")]
        public string Question { get; set; }

        [JsonProperty("answers")]
        public List<object[]> Answers { get; set; }
    }

    public class SubmitAnswerResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("correctAnswerId")]
        public uint CorrectAnswerId { get; set; }
    }

    public class PlayerResults
    {
        [JsonProperty("username")]
        public string Username { get; set; }

        [JsonProperty("correctAnswerCount")]
        public uint CorrectAnswerCount { get; set; }

        [JsonProperty("wrongAnswerCount")]
        public uint WrongAnswerCount { get; set; }

        [JsonProperty("averageAnswerTime")]
        public float AverageAnswerTime { get; set; }
    }

    public class GetGameResultsResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("results")]
        public List<PlayerResults> Results { get; set; }
    }

    public class LeaveGameResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }
}

