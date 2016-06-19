//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_SPHERE_H
#define RENDER_SPHERE_H

#include "ObjectBase.h"

namespace alex {
class Sphere :public ObjectBase {
public:
  Sphere(std::string name,
         const cv::Vec3d &position,
         const cv::Vec3d &xAxis,
         const cv::Vec3d &yAxis,
         double reflectProbability,
         const cv::Vec3d &reflectColor,
         double diffuseProbability,
         const cv::Vec3d &diffuseColor,
         double radius,
         bool isLight = false,
         std::shared_ptr<const cv::Vec3d> lightColor = nullptr
  ) : ObjectBase(name, position, xAxis, yAxis, reflectProbability, reflectColor, diffuseProbability, diffuseColor, isLight, lightColor) {
    if (radius <= 0)
      throw "Error sphere radius <= 0";
    this->radius = radius;
  }

  static std::shared_ptr<Sphere> createLight(std::string name,
                                             const cv::Vec3d &position,
                                             const cv::Vec3d &xAxis,
                                             const cv::Vec3d &yAxis,
                                             double radius,
                                             std::shared_ptr<const cv::Vec3d> lightColor) {
    return std::shared_ptr<Sphere>(new Sphere(name, position, xAxis, yAxis,
                      0, cv::Vec3d(), 0, cv::Vec3d(),
                      radius, true, lightColor));
  }

  virtual bool intersect(const Ray &ray, cv::Vec3d &intersection, cv::Vec3d &normalVecN, bool &outsideIn) const override;
  virtual bool isInner(const cv::Vec3d &point) const override;

private:
  double radius;
};
}

#endif //RENDER_SPHERE_H
