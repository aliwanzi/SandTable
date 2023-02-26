using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SandTable
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent:Component
    {
        public Vector3 Translation
        {
            get 
            {
                InternalCall.TransformComponentGetTranslation(Entity.EntityID, out Vector3 tran);
                return tran;
            }
            set 
            {
                InternalCall.TransformComponentSetTranslation(Entity.EntityID, ref value);
            }
        }
    }

    public class RigidBody2DComponent:Component
    {
        public void ApplyLinearImpulse(Vector2 vec2Impulse, Vector2 vec2Point, bool bWake)
        {
            InternalCall.RigidBody2DComponentApplyLinearImpulse(Entity.EntityID, ref vec2Impulse, ref vec2Point, bWake);
        }

        public void ApplyLinearImpulse(Vector2 vec2Impulse, bool bWake)
        {
            InternalCall.RigidBody2DComponentApplyLinearImpulseToCenter(Entity.EntityID, ref vec2Impulse, bWake);
        }
    }
}
