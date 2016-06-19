//
// Created by alexwang on 6/19/16.
//

#include "Plane.h"

bool alex::Plane::intersect(const alex::Ray &ray,
                            cv::Vec3d &intersection, cv::Vec3d &normalVecN, bool &outsideIn) const {
  double denominator = this->xAxis.dot(ray.getDirectionN());
  if (denominator == 0)
    return false;

  double t = (this->position - ray.getStartPoint()).dot(this->xAxis) / denominator;
  intersection = ray.startPoint + ray.getDirectionN() * t;
  if (t < 0)
    return false;

  outsideIn = this->xAxis.dot(ray.getDirectionN()) < 0;
  normalVecN = outsideIn ? this->xAxis : -this->xAxis;
  return true;
}

bool alex::Plane::isInner(const cv::Vec3d &point) const {
  return this->xAxis.dot(point - this->position) < 0;
}



