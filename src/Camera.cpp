//
// Created by alexwang on 6/19/16.
//

#include "Camera.h"
#include "utils/Logger.h"
#include <cmath>

#include <opencv/highgui.h>

std::shared_ptr<alex::Ray> alex::Camera::getRay(int x, int y) const {
  if (x > width || y > height)
    return nullptr;

  auto retinaCenter = this->position - this->frontN * this->imageDistance;
  auto retinaPosition = retinaCenter +
                        leftN * ((2.0 * x / this->width - 1) * this->retinaWidth) +
                        upN * ((2.0 * y / this->height - 1) * this->retinaHeight);

  double theta = randRange(0, M_PI);
  auto randVec = (leftN * cos(theta) + upN * sin(theta)) * this->apertureRadius;
  auto aperturePosition = this->position + randVec;

  double objectDis = this->focalDistance * this->imageDistance / (this->imageDistance - this->focalDistance);
  auto pointOnFocalPlane = this->position + frontN * objectDis;
  auto normalVecFocalPlaneN = this->frontN;
  Ray r(retinaPosition, this->position - retinaPosition);
  auto targetPoint = intersectWithPlane(r, pointOnFocalPlane, normalVecFocalPlaneN);
  return std::shared_ptr<Ray>(new Ray(aperturePosition, targetPoint - aperturePosition));
}

cv::Vec3d alex::Camera::renderAt(int x, int y) const {
  int sampleTimes = 10;
  std::vector<cv::Vec3d> preSamples;
  cv::Vec3d result(0, 0, 0);
  for (int i = 0; i < sampleTimes; ++i) {
    auto ray = getRay(x, y);
    auto color = this->pathTracer->trace(*ray);
    preSamples.push_back(color);
    result += color;
  }
  return result / sampleTimes;
}

double alex::Camera::randRange(double start, double end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(start, end);
  return dis(gen);
}

cv::Vec3d alex::Camera::intersectWithPlane(const Ray &ray, const cv::Vec3d &point, const cv::Vec3d &normalVecN) {
  double t = (point - ray.getStartPoint()).dot(normalVecN) / (normalVecN.dot(ray.getDirectionN()));
  return ray.getStartPoint() + ray.getDirectionN() * t;
}

void alex::Camera::startRendering() const {
  cv::Mat img(cv::Size(width, height), CV_8UC3);

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      auto color = renderAt(x, y);
      cv::Vec3b newColor;
      newColor[0] = (uint8_t) (color[0] * 255);
      newColor[1] = (uint8_t) (color[1] * 255);
      newColor[2] = (uint8_t) (color[2] * 255);

      img.at<cv::Vec3b>(y, x) = newColor;
    }
//    Log.i("camera very long tag", "line");
  }
  std::cout << img << std::endl;
  imwrite("image.png", img);
}









