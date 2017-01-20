#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "SGE/Mesh.h"

#include <glm/glm.hpp>

//! An axis aligned bounding box.
class BoundingBox {
public:
  BoundingBox(const AbstractMesh& template_mesh);
  BoundingBox(glm::vec3 min, glm::vec3 max);
  ~BoundingBox();
  inline glm::vec3 min() const {return _min;}
  inline glm::vec3 max() const {return _max;}
  bool intersects(const glm::vec3& point) const;
  std::pair<bool, float> intersects(
  	const glm::vec3& origin, const glm::vec3& direction) const;
private:
  glm::vec3 _min;
  glm::vec3 _max;
};


#endif
