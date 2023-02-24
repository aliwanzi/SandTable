using System;

namespace SandTable
{
    public class Entity
    {
        public ulong EntityID { get; set; }

        public Vector3 Translation
        {
            get
            {
                InternalCall.TransformComponentGetTranslation(EntityID, out Vector3 tran);
                return tran;
            }
            set
            {
                InternalCall.TransformComponentSetTranslation(EntityID, ref value);
            }
        }

        public Entity(ulong entityID)
        {
            EntityID = entityID;
        }

        public Entity()
        {
            EntityID = 0;
        }

        public bool HasComponent<T>() where T:Component,new()
        {
            Type componentType = typeof(T);
            return InternalCall.EntityHasComponet(EntityID, componentType);
        }

        public T GetComponent<T>() where T:Component,new()
        {
            if (!HasComponent<T>())
            {
                return null;
            }
            return new T { Entity = this };
        }
    }
}