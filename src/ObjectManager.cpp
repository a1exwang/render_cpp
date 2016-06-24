//
// Created by alexwang on 6/19/16.
//

#include "objects/ObjectBase.h"
#include "ObjectManager.h"
#include <memory>

void alex::ObjectManager::addObject(ObjectBase *object) {
  objects.push_back(object);
}

const alex::ObjectBase *alex::ObjectManager::determineIntersection(
        const Ray &ray,
        cv::Vec3d &intersection, 
        cv::Vec3d &normalVecN, 
        bool &outsideIn) const {

  double nearestDis = 10e10;
  const ObjectBase *ret = nullptr;

  cv::Vec3d nearestIntersection;
  cv::Vec3d nearestNormalVecN;
  bool nearestOutsideIn = true;
  for (auto object : objects) {
    if (object->intersect(ray, intersection, normalVecN, outsideIn)) {

      double dis = cv::norm(ray.getStartPoint() - intersection);
      if (dis < nearestDis) {
        ret = object;
        nearestDis = dis;
        nearestIntersection = intersection;
        nearestNormalVecN = normalVecN;
        nearestOutsideIn = outsideIn;
      }
    }
  }

  intersection = nearestIntersection;
  normalVecN = nearestNormalVecN;
  outsideIn = nearestOutsideIn;
  return ret;
}



