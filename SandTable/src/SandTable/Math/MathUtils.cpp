#include "pch.h"
#include "SandTable/Math/MathUtils.h"

SAND_TABLE_NAMESPACE_BEGIN

bool MathUtils::DecomposeTransform(const glm::mat4& mat4Transform, glm::vec3& vec3Translation, 
	glm::vec3& vec3Rotation, glm::vec3& vec3Scale)
{
	// From glm::decompose in matrix_decompose.inl

	using namespace glm;
	using T = float;

	mat4 LocalMatrix(mat4Transform);

	// Normalize the matrix.
	if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
		return false;

	// First, isolate perspective.  This is the messiest.
	if (
		epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
		epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
	{
		// Clear the perspective partition
		LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
		LocalMatrix[3][3] = static_cast<T>(1);
	}

	// Next take care of translation (easy).
	vec3Translation = vec3(LocalMatrix[3]);
	LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

	vec3 Row[3];

	// Now get scale and shear.
	for (length_t i = 0; i < 3; ++i)
		for (length_t j = 0; j < 3; ++j)
			Row[i][j] = LocalMatrix[i][j];

	// Compute X scale factor and normalize first row.
	vec3Scale.x = length(Row[0]);
	Row[0] = detail::scale(Row[0], static_cast<T>(1));
	vec3Scale.y = length(Row[1]);
	Row[1] = detail::scale(Row[1], static_cast<T>(1));
	vec3Scale.z = length(Row[2]);
	Row[2] = detail::scale(Row[2], static_cast<T>(1));

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip.  If the determinant
	// is -1, then negate the matrix and the scaling factors.
#if 0
	Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
	if (dot(Row[0], Pdum3) < 0)
	{
		for (length_t i = 0; i < 3; i++)
		{
			vec3Scale[i] *= static_cast<T>(-1);
			Row[i] *= static_cast<T>(-1);
		}
	}
#endif

	vec3Rotation.y = asin(-Row[0][2]);
	if (cos(vec3Rotation.y) != 0) {
		vec3Rotation.x = atan2(Row[1][2], Row[2][2]);
		vec3Rotation.z = atan2(Row[0][1], Row[0][0]);
	}
	else {
		vec3Rotation.x = atan2(-Row[2][0], Row[1][1]);
		vec3Rotation.z = 0;
	}

	return true;
}

SAND_TABLE_NAMESPACE_END

