//
// Created by alexwang on 6/19/16.
//

#include "Sphere.h"
#include <cmath>

bool alex::Sphere::intersect(const alex::Ray &ray, cv::Vec3d &intersection, cv::Vec3d &normalVecN, bool &outsideIn) const {
  double t = (this->position - ray.getStartPoint()).dot(ray.getDirectionN());
  auto nearestPoint = ray.getStartPoint() + ray.getDirectionN() * t;

  if (isInner(nearestPoint))
    return false;

  double nearestDis = cv::norm(nearestPoint - this->position);
  auto nearestPointToIntersection = sqrt(this->radius * this->radius - nearestDis * nearestDis);

  auto vec = ray.getDirectionN() * nearestPointToIntersection;
  outsideIn = !isInner(ray.getStartPoint());

  if (outsideIn && t < 0)
    return false;

  intersection = outsideIn ? nearestPoint - vec : nearestPoint + vec;
  normalVecN = outsideIn ? intersection - this->position : this->position - intersection;
  return true;
}

bool alex::Sphere::isInner(const cv::Vec3d &point) const {
  return cv::norm(point - this->position) < this->radius;
}





