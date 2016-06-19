//
// Created by alexwang on 6/19/16.
//

#include "ObjectManager.h"

void alex::ObjectManager::addObject(std::shared_ptr<ObjectBase> object) {
  objects.push_back(object);
}

bool alex::ObjectManager::determineIntersection(
        const Ray &ray,
        cv::Vec3d &intersection, 
        cv::Vec3d &normalVecN, 
        bool &outsideIn) const {
  for (auto object : objects) {
    if (object->intersect(ray, intersection, normalVecN, outsideIn))
      return true;
  }
  return false;
}



