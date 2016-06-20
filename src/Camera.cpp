//
// Created by alexwang on 6/19/16.
//

#include "Camera.h"
#include "utils/Logger.h"
#include "utils/MathUtils.h"
#include "utils/ThreadPool.h"
#include <cmath>

#include <opencv/highgui.h>

std::shared_ptr<alex::Ray> alex::Camera::getRay(int x, int y) const {
  if (x > width || y > height)
    return nullptr;

  auto retinaCenter = this->position - this->frontN * this->imageDistance;
  auto retinaPosition = retinaCenter +
                        rightN * ((2.0 * x / this->width - 1) * this->retinaWidth) +
                        upN * ((2.0 * y / this->height - 1) * this->retinaHeight);

  double theta = randRange(0, M_PI * 2);
  auto randVec = (rightN * cos(theta) + upN * sin(theta)) * this->apertureRadius;
  auto aperturePosition = this->position + randVec;

  double objectDis = this->focalDistance * this->imageDistance / (this->imageDistance - this->focalDistance);
  auto pointOnFocalPlane = this->position + frontN * objectDis;
  auto normalVecFocalPlaneN = this->frontN;
  Ray r(retinaPosition, this->position - retinaPosition);
  auto targetPoint = intersectWithPlane(r, pointOnFocalPlane, normalVecFocalPlaneN);
  return std::shared_ptr<Ray>(new Ray(aperturePosition, targetPoint - aperturePosition));
}

cv::Vec3d alex::Camera::renderAt(int x, int y) const {
  std::vector<cv::Vec3d> preSamples;
  cv::Vec3d result(0, 0, 0);
  for (int i = 0; i < this->monteCarloTimes; ++i) {
    auto ray = getRay(x, y);

    PathTrace::TraceInfo info(x, y);
    auto color = this->pathTracer->trace(*ray, &info);
    preSamples.push_back(color);
    result += color;
  }
  return result / this->monteCarloTimes;
}


cv::Vec3d alex::Camera::intersectWithPlane(const Ray &ray, const cv::Vec3d &point, const cv::Vec3d &normalVecN) {
  double t = (point - ray.getStartPoint()).dot(normalVecN) / (normalVecN.dot(ray.getDirectionN()));
  return ray.getStartPoint() + ray.getDirectionN() * t;
}

void alex::Camera::startRendering(std::size_t threads) const {
  if (threads == 0)
    renderSingleThread();
  else
    renderThreads(threads);
}

void alex::Camera::renderSingleThread() const {
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
    if (x % 20 == 0)
      std::cout << "progress " << std::setprecision(3) << 100.0 * x / width << "%" << std::endl;
  }
  imwrite("image.png", img);
}

void alex::Camera::renderThreads(size_t n) const {
  ThreadPool pool(n);

  int totalCount = width * height;

  std::shared_ptr<cv::Mat> img(new cv::Mat(cv::Size(width, height), CV_8UC3));
  std::shared_ptr<std::mutex> mu(new std::mutex);
  std::shared_ptr<int> finishedCount(new int(0));

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      pool.enqueue([x, y, totalCount, img, mu, finishedCount, this]() -> void {
        auto color = renderAt(x, y);
        cv::Vec3b newColor;
        newColor[0] = (uint8_t) (color[0] * 255);
        newColor[1] = (uint8_t) (color[1] * 255);
        newColor[2] = (uint8_t) (color[2] * 255);

        img->at<cv::Vec3b>(y, x) = newColor;

        mu->lock();
        {
          *finishedCount += 1;
          if (*finishedCount % (totalCount / 100) == 0)
            std::cout << "progress " << std::setprecision(3) << 100.0 * *finishedCount / totalCount << "%" << std::endl;

          if (*finishedCount == totalCount) {
            imwrite("image.png", *img);
            exit(0);
          }
        }
        mu->unlock();
      });
    }
  }

}













