//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_OBJECTBASE_H
#define RENDER_OBJECTBASE_H

#include <opencv/cv.h>
#include <memory>
#include "../utils/Ray.h"

namespace alex {
class ObjectBase {
public:
  ObjectBase(
          std::string name,
          const cv::Vec3d &position,
          const cv::Vec3d &xAxis, // normal vector
          const cv::Vec3d &yAxis, // up directionN
          double reflectProbability,
          const cv::Vec3d &reflectColor,
          double diffuseProbability,
          const cv::Vec3d &diffuseColor,
          bool isLight = false,
          std::shared_ptr<const cv::Vec3d> lightColor = nullptr
  ) :name(name),
     position(position),
     xAxis(xAxis),
     yAxis(yAxis),
     reflectProbability(reflectProbability),
     reflectColor(reflectColor),
     diffuseProbability(diffuseProbability),
     diffuseColor(diffuseColor),
     isLight(isLight),
     lightColor(lightColor)
  { }

  virtual bool intersect(const Ray &ray, cv::Vec3d &intersection, cv::Vec3d &normalVecN, bool &outsideIn) const = 0;
  virtual bool isInner(const cv::Vec3d &point) const = 0;

  virtual bool reflect(const Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                       cv::Vec3d &color, Ray &outRay) const;
  virtual bool diffuse(const Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                       cv::Vec3d &color, Ray &outRay) const;

  double getReflectProbability() const { return reflectProbability; }
  double getDiffuseProbability() const { return diffuseProbability; }
  bool isALight() const { return isLight; }
  std::shared_ptr<const cv::Vec3d> getLightColor() const { return lightColor; }
  std::string getName() const { return name; }

protected:
  std::string name;
  const cv::Vec3d position;
  const cv::Vec3d xAxis; // normal vector
  const cv::Vec3d yAxis; // up directionN
  double reflectProbability;
  const cv::Vec3d reflectColor;
  double diffuseProbability;
  const cv::Vec3d diffuseColor;
  bool isLight;
  std::shared_ptr<const cv::Vec3d> lightColor;
};
}


#endif //RENDER_OBJECTBASE_H
