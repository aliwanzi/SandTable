#pragma once
#include "SandTable/RayTracing/Hittable.h"
SAND_TABLE_NAMESPACE_BEGIN

class Object :public Hittable
{
public:
	Object(uint32_t uiEntitID);
	virtual ~Object() = default;
	uint32_t GetEntityID()const;

	void ResetDirty();
	bool GetDirty();

	void SetMaterialID(uint32_t uiMaterialID);
	uint32_t GetMaterialID() const;
protected:
	bool m_bDirty;
	uint32_t m_uiEntitID;
	uint32_t m_uiMaterialID;
};
SAND_TABLE_NAMESPACE_END


