#include "pch.h"
#include "XZRectangle.h"
SAND_TABLE_NAMESPACE_BEGIN

XZRectangle::XZRectangle(uint32_t uiEntitID):
    Object(uiEntitID),
    m_vec2PointMin(glm::dvec2(0.0)),
    m_vec2PointMax(glm::dvec2(0.0)),
    m_dYPos(0.0)
{
}

void XZRectangle::SetXZPoints(const glm::dvec2& PointXZ0, const glm::dvec2& PointXZ1)
{
    m_vec2PointMin = glm::min(PointXZ0, PointXZ1);
    m_vec2PointMax = glm::max(PointXZ0, PointXZ1);
    m_bDirty = true;
}

void XZRectangle::SetYPos(double dYPos)
{
    m_dYPos = dYPos;
    m_bDirty = true;
}

bool XZRectangle::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord)
{
    auto center = m_spBoundingBox->GetCentroid();
    auto root = (center.y - ray.Origin.y) / ray.Direction.y;
    if (root < fMin || root > fMax)
        return false;
    auto point = (ray.Origin + root * ray.Direction);
    if (!m_spBoundingBox->Contains(point))
    {
        return false;
    }

    auto min = m_spBoundingBox->GetMin();
    auto uvw = (point - min) / m_spBoundingBox->GetDimension();

    hitRecord.UV = glm::dvec2(uvw.x, uvw.z);
    hitRecord.HitDistance = root;
    hitRecord.SetWorldNormal(ray, glm::dvec4(0, 1, 0, 1));
    hitRecord.WorldPosition = ray.Origin + root * ray.Direction;
    hitRecord.MaterialID = m_uiMaterialID;
    hitRecord.EntityID = m_uiEntitID;

    return true;
}

bool XZRectangle::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
    m_spBoundingBox = CreateRef<BoundingBox>(glm::dvec3(m_vec2PointMin.x, m_dYPos - 0.0001, m_vec2PointMin.y),
        glm::dvec3(m_vec2PointMax.x, m_dYPos + 0.00001, m_vec2PointMax.y));
    return true;
}


SAND_TABLE_NAMESPACE_END
