using System;

namespace SandTable
{
    public class Entity
    {
        public readonly ulong EntityID;

        public Entity(ulong entityID)
        {
            EntityID = entityID;
        }

        public Entity()
        {
            EntityID = 0;
        }

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

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCall.EntityHasComponet(EntityID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
            {
                return null;
            }
            return new T { Entity = this };
        }

        public Entity FindEntityByName(string name)
        {
            ulong entityID = InternalCall.EntityFindEntityByName(name);
            if (entityID == 0)
            {
                return null;
            }
            return new Entity(entityID);
        }

        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCall.GetScriptEntityInstance(EntityID);
            return instance as T;
        }
    }
}