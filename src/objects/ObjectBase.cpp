//
// Created by alexwang on 6/19/16.
//

#include "ObjectBase.h"
#include "../utils/MathUtils.h"

bool alex::ObjectBase::reflect(const Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                               cv::Vec3d &color, Ray &outRay) const {
  double cosTheta = inRay.getDirectionN().dot(normalVecN);
  auto front = (normalVecN * (2 * cosTheta) + inRay.getDirectionN());
  outRay = Ray(intersection + normalVecN * alex::Epsilon, front);
  color = reflectColor;
  return true;
}

cv::Vec3d getAVerticalNVec(const cv::Vec3d &v) {
  if (norm(v) == 0) {
    throw "zero vector detected";
  }

  if (v[0] == 0) {
    return normalize(cv::Vec3d(0, -v[2], v[1]));
  }
  else {
    return normalize(cv::Vec3d(-(v[1] + v[2]) / v[0], 1.0, 1.0));
  }
}

bool alex::ObjectBase::diffuse(const alex::Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                               cv::Vec3d &color, alex::Ray &outRay) const {

  auto zAxis = normalVecN;
  auto yAxis = getAVerticalNVec(normalVecN);
  auto xAxis = yAxis.cross(zAxis);

  double theta = randRange(0, M_PI / 2);
  double phi = randRange(0, M_PI * 2);

  auto direction = zAxis * sin(theta) + (yAxis * cos(phi) + xAxis * sin(phi)) * cos(theta);

  outRay = Ray(intersection, direction + normalVecN * alex::Epsilon);
  color = diffuseColor;
  return true;
}
