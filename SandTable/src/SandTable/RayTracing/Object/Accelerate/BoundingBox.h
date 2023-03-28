#pragma once
#include "SandTable/RayTracing/Ray.h"
SAND_TABLE_NAMESPACE_BEGIN
class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const glm::dvec3 min, const glm::dvec3 max);

    void SetMin(const glm::dvec3& vec3Min);
    const glm::dvec3& GetMin()const;

    void SetMax(const glm::dvec3& vec3Max);
    const glm::dvec3& GetMax()const;

    bool Intersect(const Ray& ray, double& stepMin, double& stepMax) const;
    bool Contains(const glm::dvec3& point) const;
    glm::dvec3 GetDimension() const;
    glm::dvec3 GetCentroid() const;
    double GetArea() const;
    int GetMaxExtent() const;

    void Merge(Ref<BoundingBox> spBoundingBox);
    void SurrondingBox(Ref<BoundingBox> spLeftBoundingBox, Ref<BoundingBox>spRightBoundingBox);
    void Merge(const glm::dvec3& p);
    bool Valid() const;
private:

    glm::dvec3 m_vec3Min;
    glm::dvec3 m_vec3Max;
};

SAND_TABLE_NAMESPACE_END
