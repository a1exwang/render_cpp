//
// Created by alexwang on 6/19/16.
//

#include "Plane.h"

bool alex::Plane::intersect(const alex::Ray &ray,
                            cv::Vec3d &intersection, cv::Vec3d &normalVecN, bool &outsideIn) const {

  return false;
}

bool alex::Plane::isInner(const cv::Vec3d &point) const {
  return this->xAxis.dot(point - this->position) < 0;
}



