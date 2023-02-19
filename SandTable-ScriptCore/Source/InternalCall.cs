using System.Runtime.CompilerServices;

namespace SandTable
{
    public static class InternalCall
    {
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
