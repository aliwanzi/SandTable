
using System;

namespace SandTable
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate-{EntityID}");
        }
        void OnUpdate(float ts)
        {
            //Console.WriteLine($"Player.OnUpdate:{ts}");

            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.Y = 1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y = -1.0f;
            }

            if (Input.IsKeyDown(KeyCode.A))
            {
                velocity.X = -1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.X = 1.0f;
            }

            velocity *= speed;

            var translation = Translation;
            translation += velocity * ts;
            Translation = translation;

            Console.WriteLine($"Player.OnUpdate:{Translation.X}");
        }
    }
}
