#pragma once
#include <xhash>
SAND_TABLE_NAMESPACE_BEGIN

class UUID
{
public:
	UUID();
	UUID(uint64_t uiUUID);
	UUID(const UUID&) = default;
	operator uint64_t() const;
private:
	uint64_t m_uiUUID;
};

SAND_TABLE_NAMESPACE_END

namespace std
{
	template<>
	struct hash<SandTable::UUID>
	{
		std::size_t operator()(const SandTable::UUID& uuid)
		{
			return static_cast<uint64_t>(uuid);
		}
	};
}