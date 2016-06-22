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
         const std::string &brdfFilePath,
         const cv::Vec3d &position,
         const cv::Vec3d &xAxis,
         const cv::Vec3d &yAxis,
         double reflectProbability,
         const cv::Vec3d &reflectColor,
         double diffuseProbability,
         const cv::Vec3d &diffuseColor,
         double refractProbability,
         const cv::Vec3d &refractColor,
         double refractive,
         double radius,
         bool isLight = false,
         std::shared_ptr<const cv::Vec3d> lightColor = nullptr
  ) : ObjectBase(name, brdfFilePath, position, xAxis, yAxis,
                 reflectProbability, reflectColor,
                 diffuseProbability, diffuseColor,
                 refractProbability, refractColor, refractive,
                 isLight, lightColor) {
    if (radius <= 0)
      throw "Error sphere radius <= 0";
    this->radius = radius;
  }

  static std::shared_ptr<Sphere> createLight(std::string name,
                                             std::string brdfFilePath,
                                             const cv::Vec3d &position,
                                             const cv::Vec3d &xAxis,
                                             const cv::Vec3d &yAxis,
                                             double radius,
                                             const cv::Vec3d &lightColor) {
    return std::shared_ptr<Sphere>(
            new Sphere(name, brdfFilePath, position, xAxis, yAxis,
                      0, cv::Vec3d(), 0, cv::Vec3d(), 0, cv::Vec3d(), 1,
                      radius, true, std::shared_ptr<const cv::Vec3d>(new cv::Vec3d(lightColor))));
  }

  virtual bool intersect(const Ray &ray, cv::Vec3d &intersection, cv::Vec3d &normalVecN, bool &outsideIn) const override;
  virtual bool isInner(const cv::Vec3d &point) const override;

private:
  double radius;
};
}

#endif //RENDER_SPHERE_H
