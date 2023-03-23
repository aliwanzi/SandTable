#pragma once
#include "SandTable/RayTracing/Hittable.h"
SAND_TABLE_NAMESPACE_BEGIN
class Accelerate:public Hittable
{
public:
	Accelerate();
	virtual ~Accelerate() = default;
	virtual const Ref<BoundingBox>& GetBoundingBox()const override;
protected:
	Ref<BoundingBox> m_spBoundingBox;
};
SAND_TABLE_NAMESPACE_END
