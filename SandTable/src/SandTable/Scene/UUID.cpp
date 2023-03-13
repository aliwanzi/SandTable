#include "pch.h"
#include "UUID.h"
#include "SandTable/Math/Random.h"

SAND_TABLE_NAMESPACE_BEGIN

UUID::UUID():
	m_uiUUID(Random::Uint64())
{

}

UUID::UUID(uint64_t uiUUID)
{
	m_uiUUID = uiUUID;
}

UUID::operator uint64_t() const
{
	return m_uiUUID;
}

SAND_TABLE_NAMESPACE_END
