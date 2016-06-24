//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_OBJECTMANAGER_H
#define RENDER_OBJECTMANAGER_H

#include "objects/ObjectBase.h"
#include <memory>
#include <vector>
#include <opencv/cv.h>

namespace alex {
class ObjectManager {
public:
  ObjectManager() { }
  ~ObjectManager() { for (auto ptr : objects) delete ptr; }
  void addObject(ObjectBase *object);

  const ObjectBase *determineIntersection(const Ray &ray,
                             cv::Vec3d &intersection, 
                             cv::Vec3d &normalVecN, 
                             bool &outsideIn) const;
private:
  std::vector<ObjectBase *> objects;
};
}


#endif //RENDER_OBJECTMANAGER_H
