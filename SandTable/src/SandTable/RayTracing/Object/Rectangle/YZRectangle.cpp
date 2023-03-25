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
    auto root = (m_dXPos - (ray.Origin - m_spTransform->GetTranslate()).x) / ray.Direction.x;
    if (root < fMin || root > fMax)
        return false;

    auto point = (ray.Origin + root * ray.Direction);
    if (!m_spBoundingBox->Contains(point))
    {
        return false;
    }

    hitRecord.UV = (glm::dvec2(point.y, point.z) - m_vec2PointMin) / (m_vec2PointMax - m_vec2PointMin);
    hitRecord.HitDistance = root;
    hitRecord.SetWorldNormal(ray, glm::dvec3(1, 0, 0));
    hitRecord.WorldPosition = ray.Origin+m_spTransform->GetTranslate() + ray.Direction * hitRecord.HitDistance;
    hitRecord.MaterialID = m_uiMaterialID;
    hitRecord.EntityID = m_uiEntitID;

    return true;
}

bool YZRectangle::CreateBoundingBox(double dStepBegin, double dStepEnd)
{
    m_spBoundingBox = CreateRef<BoundingBox>(glm::dvec3(m_dXPos - 0.0001, m_vec2PointMin), glm::dvec3(m_dXPos + 0.00001, m_vec2PointMax));
    m_spBoundingBox->MakeTranslate(m_spTransform->GetTranslate());
    return true;
}

SAND_TABLE_NAMESPACE_END
