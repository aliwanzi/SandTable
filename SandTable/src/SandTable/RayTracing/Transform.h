#pragma once

SAND_TABLE_NAMESPACE_BEGIN
class Transform
{
public:
	Transform();
	void SetTranslate(const glm::dvec3& vec3Translate);
	const glm::dvec3& GetTranslate()const;

	const glm::highp_dmat4& GetRotation()const;
	const glm::highp_dmat4& GetRotationInverse()const;

	const glm::highp_dmat4& GetTranslation()const;
	const glm::highp_dmat4& GetTranslationInverse()const;

	void SetRotateY(double fAngle);
	double GetRotateY()const;
private:
	glm::dvec3 m_vec3Translate;
	double m_fRotateY;

	glm::highp_dmat4 m_mat4Rotation;
	glm::highp_dmat4 m_mat4RotationInverse;

	glm::highp_dmat4 m_mat4Translation;
	glm::highp_dmat4 m_mat4TranslationInverse;
};

SAND_TABLE_NAMESPACE_END

