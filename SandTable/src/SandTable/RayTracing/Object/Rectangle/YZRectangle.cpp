#include "pch.h"
#include "YZRectangle.h"
SAND_TABLE_NAMESPACE_BEGIN

YZRectangle::YZRectangle(uint32_t uiEntitID):
    Object(uiEntitID)
{
}

void YZRectangle::SetYZPoints(const glm::dvec2& PointYZ0, const glm::dvec2& PointYZ1)
{
    m_vec2PointMin = glm::min(PointYZ0, PointYZ1);
    m_vec2PointMax = glm::max(PointYZ0, PointYZ1);
    m_bDirty = true;
}

void YZRectangle::SetXPos(double dXPos)
{
    m_dXPos = dXPos;
    m_bDirty = true;
}

bool YZRectangle::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord)
{
    Ray transRay;
    transRay.Origin =  m_spTransform->GetRotationInverse()*m_spTransform->GetTranslationInverse()
        * glm::dvec4(ray.Origin, 1.0);
    transRay.Direction = m_spTransform->GetRotationInverse() * glm::dvec4(ray.Direction, 1.0);

    auto center = m_spBoundingBox->GetCentroid();
    auto root = (center.x - transRay.Origin.x) / transRay.Direction.x;
    if (root < fMin || root > fMax)
        return false;
	auto point = (transRay.Origin + root * transRay.Direction);
	if (!m_spBoundingBox->Contains(point))
		return false;

    auto min = m_spBoundingBox->GetMin();
    auto uvw = (point - min) / m_spBoundingBox->GetDimension();

    auto normal = m_spTransform->GetRotation() * glm::dvec4(1, 0, 0, 1);
    hitRecord.UV = glm::dvec2(uvw.y, uvw.z);
    hitRecord.HitDistance = root;
    hitRecord.SetWorldNormal(ray, normal);
    hitRecord.WorldPosition = ray.Origin + root * ray.Direction;
    hitRecord.MaterialID = m_uiMaterialID;
    hitRecord.EntityID = m_uiEntitID;

    return true;
}

bool YZRectangle::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
    m_spBoundingBox = CreateRef<BoundingBox>(glm::dvec3(m_dXPos - 0.0001, m_vec2PointMin), glm::dvec3(m_dXPos + 0.00001, m_vec2PointMax));
    return true;
}

SAND_TABLE_NAMESPACE_END
