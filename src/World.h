//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_WORLD_H
#define RENDER_WORLD_H

#include "objects/ObjectBase.h"
#include "ObjectManager.h"
#include <vector>
#include <memory>

namespace alex {
class World {
public:
  void addObject(ObjectBase *object) { objectManager.addObject(object); }

  const ObjectBase *determineIntersection(const Ray &ray,
                             cv::Vec3d &intersection,
                             cv::Vec3d &normalVecN,
                             bool &outsideIn) const {
    return objectManager.determineIntersection(ray, intersection, normalVecN, outsideIn);
  }

private:
  ObjectManager objectManager;
};
}




#endif //RENDER_WORLD_H
