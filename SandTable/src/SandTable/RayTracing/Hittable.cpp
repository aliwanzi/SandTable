#include "pch.h"
#include "Hittable.h"

SAND_TABLE_NAMESPACE_BEGIN
const Ref<BoundingBox>& Hittable::GetBoundingBox() const
{
	return m_spBoundingBox;
}

SAND_TABLE_NAMESPACE_END