namespace SandTable
{
    public struct Vector2
    {
        public float X { get; set; }
        public float Y { get; set; }
        public static Vector2 Zero => new Vector2(0.0f);

        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public static Vector2 operator +(Vector2 vector0, Vector2 vector1)
        {
            return new Vector2(vector0.X + vector1.X, vector0.Y + vector1.Y);
        }

        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }
    }
}
