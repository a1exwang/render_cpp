//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include "utils/Ray.h"
#include "PathTrace.h"
#include <opencv/cv.h>
#include <memory>
#include <mutex>


namespace alex {
class Camera {
public:
  Camera(std::shared_ptr<const World> world,
         const cv::Vec3d &position,
         const cv::Vec3d &front,
         const cv::Vec3d &up,
         double retinaWidth,
         double retinaHeight,
         int width,
         int height,
         double imageDistance,
         double apertureRadius,
         double focalDistance,
         int monteCarloTimes,
         int maxTraceDepth,
         double finalColorIndex
  ) :world(world),
     pathTracer(new PathTrace(*world, maxTraceDepth)),
     position(position),
     frontN(normalize(front)),
     upN(normalize(up)),
     retinaWidth(retinaWidth),
     retinaHeight(retinaHeight),
     width(width),
     height(height),
     imageDistance(imageDistance),
     apertureRadius(apertureRadius),
     focalDistance(focalDistance),
     monteCarloTimes(monteCarloTimes),
     finalColorIndex(finalColorIndex) {
    rightN = frontN.cross(upN);
  }

  int getWidth() const { return width; }
  int getHeight() const { return height; }
  std::shared_ptr<Ray> getRay(int x, int y) const;
  cv::Vec3d renderAt(int x, int y) const;

  template<class ElementT>
  static void serializeMat(std::string filePath, const cv::Mat &mat);
  template<class ElementT>
  static std::shared_ptr<cv::Mat> deserializeMat(std::string filePath);

  void startRendering(std::size_t threads = 0) const;

private:
  static cv::Vec3d intersectWithPlane(const Ray &ray, const cv::Vec3d &, const cv::Vec3d &);

  void renderSingleThread() const;
  void renderThreads(size_t n) const;
private:
  std::shared_ptr<const World> world;
  std::shared_ptr<PathTrace> pathTracer;

  const cv::Vec3d position;
  const cv::Vec3d frontN;
  const cv::Vec3d upN;
  cv::Vec3d rightN;
  double retinaWidth;
  double retinaHeight;
  int width;
  int height;
  double imageDistance;
  double apertureRadius;
  double focalDistance;
  int monteCarloTimes;
  double finalColorIndex;
};
}



#endif //RENDER_CAMERA_H
