//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include "helpers/Ray.h"
#include <opencv/cv.h>
#include <memory>


namespace alex {
class Camera {
public:
  Camera(const cv::Vec3d &position,
         const cv::Vec3d &front,
         const cv::Vec3d &up,
         double retinaWidth,
         double retinaHeight,
         int width,
         int height,
         double imageDistance,
         double apertureRadius,
         double focalDistance
  ) :position(position),
     frontN(normalize(front)),
     upN(normalize(up)),
     retinaWidth(retinaWidth),
     retinaHeight(retinaHeight),
     width(width),
     height(height),
     imageDistance(imageDistance),
     apertureRadius(apertureRadius),
     focalDistance(focalDistance)
  {
    leftN = front.cross(leftN);
  }

  int getWidth() const { return width; }
  int getHeight() const { return height; }
  std::shared_ptr<Ray> getRay(int x, int y) const;
  cv::Vec3d renderAt(int x, int y) const;

private:
  static double randRange(double start, double end);
  static cv::Vec3d intersectWithPlane(const Ray &ray, const cv::Vec3d &, const cv::Vec3d &);

private:
  const cv::Vec3d position;
  const cv::Vec3d frontN;
  const cv::Vec3d upN;
  cv::Vec3d leftN;
  double retinaWidth;
  double retinaHeight;
  int width;
  int height;
  double imageDistance;
  double apertureRadius;
  double focalDistance;
};
}



#endif //RENDER_CAMERA_H