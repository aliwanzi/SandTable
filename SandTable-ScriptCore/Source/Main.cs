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
        public void PrintInt(int value)
        {
            Console.WriteLine($"c# says {value}");
        }
        public void PrintInts(int value1,int value2)
        {
            Console.WriteLine($"c# says {value1} and {value2}");
        }
    }
}