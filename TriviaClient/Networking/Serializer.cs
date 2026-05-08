using Newtonsoft.Json;

namespace TriviaClient.Networking
{
    public static class Serializer
    {
        // Converts a C# object (like LoginRequest) into a JSON string.
        public static string Serialize<T>(T obj)
        {
            return JsonConvert.SerializeObject(obj);
        }

        // Converts a JSON string back into a C# object (like LoginResponse).
        public static T Deserialize<T>(string json)
        {
            var result = JsonConvert.DeserializeObject<T>(json);
            if (result is null)
                throw new InvalidOperationException("Deserialization returned null.");
            return result;
        }
    }
}
