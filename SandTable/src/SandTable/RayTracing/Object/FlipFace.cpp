#include "pch.h"
#include "FlipFace.h"
SAND_TABLE_NAMESPACE_BEGIN

FlipFace::FlipFace(Ref<Object> spObject):m_spObject(spObject)
{
}

bool FlipFace::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord)
{
	if (!m_spObject->Hit(ray, fMin, fMax, hitRecord))
	{
		return false;
	}
	hitRecord.FrontFace = !hitRecord.FrontFace;
	return true;
}

bool FlipFace::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
	return m_spObject->CreateBoundingBox(dStepBegin, dStepEnd);
}

void FlipFace::SetMaterialID(uint32_t uiMaterialID)
{
	m_spObject->SetMaterialID(uiMaterialID);
}

SAND_TABLE_NAMESPACE_END
