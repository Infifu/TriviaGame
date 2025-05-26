using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using WPFTEST.Services;

namespace TriviaClient.Services
{
    public class Deserializer
    {
        public T deserializeJson<T>(ServerAnswer answer)
        {
            return JsonSerializer.Deserialize<T>(answer.json);
        }
    }
}
