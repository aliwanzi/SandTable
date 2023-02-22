using System;

namespace SandTable
{
    public class Entity
    {
        protected ulong EntityID { get; set; }

        public Vector3 Translation
        {
            get
            {
                InternalCall.EntityGetTranslation(EntityID, out Vector3 tran);
                return tran;
            }
            set
            {
                InternalCall.EntitySetTranslation(EntityID, ref value);
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

    }
}