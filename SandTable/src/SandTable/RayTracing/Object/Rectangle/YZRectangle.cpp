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
    auto center = m_spBoundingBox->GetCentroid();
    auto root = (center.x - ray.Origin.x) / ray.Direction.x;
    if (root < fMin || root > fMax)
        return false;
    auto point = (ray.Origin + root * ray.Direction);
    if (!m_spBoundingBox->Contains(point))
        return false;

    auto min = m_spBoundingBox->GetMin();
    auto uvw = (center - min) / m_spBoundingBox->GetDimension();

    hitRecord.UV = glm::dvec2(uvw.y, uvw.z);
    hitRecord.HitDistance = root;
    hitRecord.SetWorldNormal(ray, glm::dvec3(1, 0, 0));
    hitRecord.WorldPosition = point;
    hitRecord.MaterialID = m_uiMaterialID;
    hitRecord.EntityID = m_uiEntitID;

    return true;
}

bool YZRectangle::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
    m_spBoundingBox = CreateRef<BoundingBox>(glm::dvec3(m_dXPos - 0.0001, m_vec2PointMin), glm::dvec3(m_dXPos + 0.00001, m_vec2PointMax));
    m_spBoundingBox->MakeTranslate(m_spTransform->GetTranslate());
    m_spBoundingBox->MakeRotation(m_spTransform->GetRotation());
    return true;
}

SAND_TABLE_NAMESPACE_END
