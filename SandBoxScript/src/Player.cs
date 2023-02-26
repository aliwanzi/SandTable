using System;

namespace SandTable
{
    public class Player : Entity
    {
        private TransformComponent Transform;
        private RigidBody2DComponent RigidBody2D;

        public float Speed;
        public Vector3 Velocity;

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate-{EntityID}");
            Transform = GetComponent<TransformComponent>();
            RigidBody2D = GetComponent<RigidBody2DComponent>();

            Speed = 2.5f;
        }

        void OnUpdate(float ts)
        {
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

            RigidBody2D.ApplyLinearImpulse(velocity.XY, true);

            //var translation = Transform.Translation;
            //translation += velocity * ts;
            //Transform.Translation = translation;
        }
    }
}
