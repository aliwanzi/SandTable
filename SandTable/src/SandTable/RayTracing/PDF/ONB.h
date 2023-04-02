#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class ONB
{
public:
	ONB(const glm::dvec3& normal);
	glm::dvec3 Local(const glm::dvec3& vector)const;

	const glm::dvec3& GetU()const;
	const glm::dvec3& GetV()const;
	const glm::dvec3& GetW()const;

private:
	glm::dvec3 m_U;
	glm::dvec3 m_V;
	glm::dvec3 m_W;
};
SAND_TABLE_NAMESPACE_END

