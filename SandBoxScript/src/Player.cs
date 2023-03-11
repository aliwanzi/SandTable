using System;

namespace SandTable
{
    public class Player : Entity
    {
        private TransformComponent Transform;
        private RigidBody2DComponent RigidBody2D;

        public float Speed;
        public float Time = 0.0f;

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate-{EntityID}");
            Transform = GetComponent<TransformComponent>();
            RigidBody2D = GetComponent<RigidBody2DComponent>();

            Speed = 2.5f;
        }

        void OnUpdate(float ts)
        {
			Time += ts;
            // Console.WriteLine($"Player.OnUpdate: {ts}");

            float speed = Speed;
			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 5.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -5.0f;

			if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -5.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 5.0f;

            if(Input.IsKeyDown(KeyCode.Space))
                velocity.Y = 10.0f;

            Entity cameraEntity = FindEntityByName("Camera");
            if (cameraEntity != null)
            {
                Camera camera = cameraEntity.As<Camera>();

                if (Input.IsKeyDown(KeyCode.Q))
                    camera.DistanceFromPlayer += speed * 2.0f * ts;
                else if (Input.IsKeyDown(KeyCode.E))
                    camera.DistanceFromPlayer -= speed * 2.0f * ts;
            }

            velocity *= speed * ts;
			RigidBody2D.ApplyLinearImpulse(velocity.XY, true);

            //var translation = Transform.Translation;
            //translation += velocity * ts;
            //Transform.Translation = translation;
        }
    }
}
