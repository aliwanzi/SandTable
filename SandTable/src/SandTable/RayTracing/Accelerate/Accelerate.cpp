#include "pch.h"
#include "Accelerate.h"
SAND_TABLE_NAMESPACE_BEGIN

Accelerate::Accelerate():m_spBoundingBox(nullptr)
{
}

const Ref<BoundingBox>& Accelerate::GetBoundingBox() const
{
	return m_spBoundingBox;
}

SAND_TABLE_NAMESPACE_END
