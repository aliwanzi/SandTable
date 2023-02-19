using System;

namespace SandTable
{
    public class Entity
    {
        public Entity()
        {
            Console.WriteLine("Main constructor!");

            InternalCall.CppFunction();

            InternalCall.NativeLogInt("NativeLog", 1024);

            Vector3 pos = new Vector3(5, 2.5f, 1);
            InternalCall.NativeLogVec3(ref pos, out Vector3 result);
            Console.WriteLine($"{result.X}, {result.Y}, {result.Z}");
            Console.WriteLine("{0}", InternalCall.NativeLogVec3Dot(ref pos));
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