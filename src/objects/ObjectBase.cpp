//
// Created by alexwang on 6/19/16.
//

#include "ObjectBase.h"
#include "../utils/MathUtils.h"

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

  double cosTheta = randRange(0, 1);
  auto theta = acos(cosTheta);

  //double theta = randRange(0, M_PI / 2);
  double phi = randRange(0, M_PI * 2);

  auto direction = zAxis * sin(theta) + (yAxis * cos(phi) + xAxis * sin(phi)) * cos(theta);

  outRay = Ray(intersection + normalVecN * alex::Epsilon, direction);
  color = diffuseColor;
  return true;
}

bool alex::ObjectBase::reflect(const Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                               cv::Vec3d &color, Ray &outRay) const {
  double cosTheta = -inRay.getDirectionN().dot(normalVecN);
  auto front = (normalVecN * (2 * cosTheta) + inRay.getDirectionN());
  outRay = Ray(intersection + normalVecN * alex::Epsilon, front);
  color = reflectColor;
  return true;
}

bool alex::ObjectBase::refract(const Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                               cv::Vec3d &color, Ray &outRay) const {
  double cosI = inRay.getDirectionN().dot(normalVecN);
  double sinI = sqrt(1 - cosI * cosI);
  double sinR = sinI / refractive;

  // 全反射
  if (sinR > 1)
    return false;


  if (cosI == 0) {
    // 不发生折射, 直线投射
    outRay = Ray(intersection - normalVecN * alex::Epsilon, -normalVecN);
  }
  else {
    // 计算垂直于法线, 与入射光,法线在同一平面, 与入射光成锐角的单位向量horizontalN
    auto horizontalN = cv::normalize((inRay.getDirectionN() / cosI - normalVecN));

    double r = asin(sinR);
    auto refractDirection =
            (normalVecN * (-cos(r)) + (horizontalN) * sinR);

    outRay = Ray(intersection - normalVecN * alex::Epsilon, refractDirection);
  }


  color = refractColor;
  return true;
}

cv::Vec3d alex::ObjectBase::randomChooseRayByBRDFDistribution(const cv::Vec3d &normalVecN) const {
  auto zAxis = normalVecN;
  auto yAxis = getAVerticalNVec(normalVecN);
  auto xAxis = yAxis.cross(zAxis);

  double theta = randRange(0, M_PI / 2);
  double phi = randRange(0, M_PI * 2);

  auto direction = zAxis * sin(theta) + (yAxis * cos(phi) + xAxis * sin(phi)) * cos(theta);

  return direction;
}

bool alex::ObjectBase::brdf(const Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                            cv::Vec3d &color, Ray &outRay) const {

  outRay = Ray(intersection + normalVecN * alex::Epsilon, randomChooseRayByBRDFDistribution(normalVecN));
  double thetaOut = acos(outRay.getDirectionN().dot(normalVecN));
  this->brdfPtr->readColor(acos(inRay.getDirectionN().dot(-normalVecN)), 0, thetaOut, M_PI, color);
  return true;
}






