namespace TriviaClient.State
{
    public static class SessionData
    {
        // Who is currently logged in?
        public static string Username { get; set; }

        // Which room are they in? (-1 means they are in the menu)
        public static int RoomId { get; set; } = -1;
        public static bool IsAdmin { get; set; }
        public static int QuestionCount { get; set; } = 5; 
        public static int AnswerTimeout { get; set; } = 10;

        // Call this when the user logs out
        public static void Clear()
        {
            Username = null;
            RoomId = -1;
            IsAdmin = false;
        }
    }
}