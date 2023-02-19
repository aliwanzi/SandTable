
using System;

namespace SandTable
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine("Player.OnCreate");
        }
        void OnUpdate(float ts)
        {
            Console.WriteLine($"Player.OnUpdate:{ts}");
        }
    }
}
