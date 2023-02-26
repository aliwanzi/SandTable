using System;
using System.Runtime.CompilerServices;

namespace SandTable
{
    public static class InternalCall
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool EntityHasComponet(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponentGetTranslation(ulong entityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponentSetTranslation(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponentApplyLinearImpulse(ulong entityID, ref Vector2 vec2Impulse, ref Vector2 vec2Point, bool bWake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponentApplyLinearImpulseToCenter(ulong entityID, ref Vector2 vec2Impulse, bool bWake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool InputIsKeyDown(KeyCode keyCode);


    }
}
