//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_PLANE_H
#define RENDER_PLANE_H

#include "ObjectBase.h"

namespace alex {
class Plane :public ObjectBase {

public:
  Plane(const std::string &name,
        const std::string &brdfFilePath,
        const cv::Vec3d &position,
        const cv::Vec3d &xAxis,
        const cv::Vec3d &yAxis,
        double reflectProbability,
        const cv::Vec3d &reflectColor,
        double diffuseProbability,
        const cv::Vec3d &diffuseColor,
        double refractProbability = 0,
        const cv::Vec3d &refractColor = cv::Vec3d(0, 0, 0),
        double refractive = 1,
        bool isLight = false,
        const std::shared_ptr<const cv::Vec3d> &lightColor = nullptr
  ) : ObjectBase(
          name, brdfFilePath, position, xAxis, yAxis,
          reflectProbability, reflectColor,
          diffuseProbability, diffuseColor,
          refractProbability, refractColor, refractive,
          isLight, lightColor) {
  }

  virtual bool intersect(const Ray &ray, cv::Vec3d &intersection, cv::Vec3d &normalVecN, bool &outsideIn) const override;
  virtual bool isInner(const cv::Vec3d &point) const override;

};
}


#endif //RENDER_PLANE_H
