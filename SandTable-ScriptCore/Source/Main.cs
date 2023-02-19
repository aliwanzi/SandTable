using System;
using System.Runtime.CompilerServices;

namespace SandTable
{
    struct Vector3
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public class Main
    {
        public Main()
        {
            Console.WriteLine("Main constructor!");

            CppFunction();

            NativeLogInt("NativeLog", 1024);

            Vector3 pos = new Vector3(5, 2.5f, 1);
            NativeLogVec3(ref pos, out Vector3 result);
            Console.WriteLine($"{result.X}, {result.Y}, {result.Z}");
            Console.WriteLine("{0}", NativeLogVec3Dot(ref pos));
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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CppFunction();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogInt(string sLog, int par);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVec3(ref Vector3 par, out Vector3 result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeLogVec3Dot(ref Vector3 par);
    }
}