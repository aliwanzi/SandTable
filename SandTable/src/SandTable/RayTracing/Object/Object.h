#pragma once
#include "SandTable/RayTracing/Hittable.h"
#include "SandTable/RayTracing/Transform.h"
SAND_TABLE_NAMESPACE_BEGIN

class Object :public Hittable
{
public:
	Object(uint32_t uiEntitID);
	virtual ~Object() = default;
	uint32_t GetEntityID()const;

	void ResetDirty();
	bool GetDirty();

	virtual void SetMaterialID(uint32_t uiMaterialID);
	uint32_t GetMaterialID() const;

	void SetTranslate(const glm::dvec3& vec3Translate);
	void SetRotateY(double angle);

	virtual const Ref<BoundingBox>& GetBoundingBox()const override;
protected:
	bool m_bDirty;
	uint32_t m_uiEntitID;
	uint32_t m_uiMaterialID;
	Ref<BoundingBox> m_spBoundingBox;
	Ref<Transform> m_spTransform;
};
SAND_TABLE_NAMESPACE_END


