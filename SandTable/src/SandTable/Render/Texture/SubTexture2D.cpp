#include "pch.h"
#include "SubTexture2D.h"

SAND_TABLE_NAMESPACE_BEGIN
SubTexture2D::SubTexture2D(const Ref<Texture>& spTexture, const glm::vec2& vec2Coords,
	const glm::vec2& vec2TileSize, const glm::vec2& vec2Offset):
	m_spTexture(spTexture)
{
	float fXMin = vec2Coords.x * vec2TileSize.x / m_spTexture->GetWidth();
	float fYMin = vec2Coords.y * vec2TileSize.y / m_spTexture->GetHeight();
	float fXMax = (vec2Coords.x + vec2Offset.x) * vec2TileSize.x / m_spTexture->GetWidth();
	float fYMax = (vec2Coords.y + vec2Offset.y) * vec2TileSize.y / m_spTexture->GetHeight();

	m_arrTexCoord[0] = glm::vec2(fXMin, fYMin);
	m_arrTexCoord[1] = glm::vec2(fXMax, fYMin);
	m_arrTexCoord[2] = glm::vec2(fXMax, fYMax);
	m_arrTexCoord[3] = glm::vec2(fXMin, fYMax);
}

const std::array<glm::vec2, 4>& SubTexture2D::GetTexCoord() const
{
	return m_arrTexCoord;
}

const Ref<Texture>& SubTexture2D::GetTexture() const
{
	return m_spTexture;
}
SAND_TABLE_NAMESPACE_END


