#include "SGE/BoundingBox.h"

BoundingBox::BoundingBox(const AbstractMesh& template_mesh) :
  _min(template_mesh.computeMinPosition()),
  _max(template_mesh.computeMaxPosition())
{
}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max) :
  _min(min),
  _max(max)
{
  
}

BoundingBox::~BoundingBox()
{
  
}

bool BoundingBox::intersects(glm::vec3 point) const
{
  return (point.x > _min.x &&
          point.y > _min.y &&
          point.z > _min.z &&
          point.x < _max.x &&
          point.y < _max.y &&
          point.z < _max.z);
}

bool BoundingBox::intersects(glm::vec3 origin, glm::vec3 direction, float* t) const
{
  // r.dir is unit direction vector of ray
  glm::vec3 dirfrac(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);
  // lb is the corner of AABB with minimal coordinates - left bottom, 
  // rt is maximal corner
  // r.org is the origin of ray
  float t1 = (_min.x - origin.x)*dirfrac.x;
  float t2 = (_max.x - origin.x)*dirfrac.x;
  float t3 = (_min.y - origin.y)*dirfrac.y;
  float t4 = (_max.y - origin.y)*dirfrac.y;
  float t5 = (_min.z - origin.z)*dirfrac.z;
  float t6 = (_max.z - origin.z)*dirfrac.z;

  float tmin = glm::max(
    glm::max(glm::min(t1, t2), glm::min(t3, t4)),
    glm::min(t5, t6));
  float tmax = glm::min(
    glm::min(glm::max(t1, t2), glm::max(t3, t4)),
    glm::max(t5, t6));

  // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
  if (tmax < 0)
  {
      *t = tmax;
      return false;
  }

  // if tmin > tmax, ray doesn't intersect AABB
  if (tmin > tmax)
  {
      *t = tmax;
      return false;
  }

  *t = tmin;
  return true;
  return false;
}