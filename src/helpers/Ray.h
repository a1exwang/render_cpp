//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_RAY_H
#define RENDER_RAY_H

#include <opencv/cv.h>

namespace alex {
class Ray {
public:
  Ray(const cv::Vec3d& startPoint, const cv::Vec3d& _direction) :startPoint(startPoint) {
    this->directionN = cv::normalize(_direction);
  }

  const cv::Vec3d &getDirectionN() const { return directionN; };
  const cv::Vec3d &getStartPoint() const { return startPoint; };

  cv::Vec3d startPoint;
  cv::Vec3d directionN; // this is normalized by default
};
}


#endif //RENDER_RAY_H
