#pragma once
#include "SandTable/Render/Texture/Texture2D.h"

SAND_TABLE_NAMESPACE_BEGIN
class SubTexture2D
{
public:
	SubTexture2D(const Ref<Texture>& spTexture, const glm::vec2& vec2Coords,
		const glm::vec2& vec2TileSize, const glm::vec2& vec2Offset = glm::vec2(1.f));
	const std::array<glm::vec2, 4>& GetTexCoord()const;
	const Ref<Texture>& GetTexture()const;
private:
	std::array<glm::vec2, 4> m_arrTexCoord;
	Ref<Texture> m_spTexture;
};
SAND_TABLE_NAMESPACE_END


