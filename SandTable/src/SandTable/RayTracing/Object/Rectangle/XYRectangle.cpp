#include "pch.h"
#include "XYRectangle.h"
SAND_TABLE_NAMESPACE_BEGIN

XYRectangle::XYRectangle(uint32_t uiEntitID):
    Object(uiEntitID),
    m_dZPos(0.0),
    m_vec2PointMin(glm::dvec2(0.0)),
    m_vec2PointMax(glm::dvec2(0.0))
{
}

void XYRectangle::SetXYPoints(const glm::dvec2& PointXY0, const glm::dvec2& PointXY1)
{
    m_vec2PointMin = glm::min(PointXY0, PointXY1);
    m_vec2PointMax = glm::max(PointXY0, PointXY1);
    m_bDirty = true;
}

void XYRectangle::SetZPos(double dZPos)
{
    m_dZPos = dZPos;
    m_bDirty = true;
}

bool XYRectangle::Hit(const Ray& ray, double fMin, double fMax, HitRecord& hitRecord)
{
    auto center = m_spBoundingBox->GetCentroid();
    auto root = (center.z - ray.Origin.z) / ray.Direction.z;
    if (root < fMin || root > fMax)
        return false;
    auto point = (ray.Origin + root * ray.Direction);
    if (!m_spBoundingBox->Contains(point))
    {
        return false;
    }

    auto min = m_spBoundingBox->GetMin();
    auto uvw = (point - min) / m_spBoundingBox->GetDimension();

    hitRecord.UV = glm::dvec2(uvw.x, uvw.y);
    hitRecord.HitDistance = root;
    hitRecord.SetWorldNormal(ray, glm::dvec4(0, 0, 1, 1));
    hitRecord.WorldPosition = ray.Origin + root * ray.Direction;
    hitRecord.MaterialID = m_uiMaterialID;
    hitRecord.EntityID = m_uiEntitID;

    return true;
}

bool XYRectangle::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
    m_spBoundingBox = CreateRef<BoundingBox>(glm::dvec3(m_vec2PointMin, m_dZPos - 0.0001), 
        glm::dvec3(m_vec2PointMax, m_dZPos + 0.00001));
    return true;
}

SAND_TABLE_NAMESPACE_END
