using System;

namespace SandTable
{
    public class Main
    {
        public float FloatVar { get; set; }
        public Main()
        {
            Console.WriteLine("Main constructor!");
        }
        public void PrintMessage()
        {
            Console.WriteLine("Hello world from c#!");
        }
        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"c# says {message}");
        }
    }
}