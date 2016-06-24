//
// Created by alexwang on 6/19/16.
//

#include "Camera.h"
#include "utils/Logger.h"
#include "utils/MathUtils.h"
#include "utils/ThreadPool.h"
#include <cmath>
#include <algorithm>

#include <opencv/highgui.h>
#include <fstream>

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
  cv::Mat imgDouble(cv::Size(width, height), CV_64FC3);

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      imgDouble.at<cv::Vec3d>(y, x) = renderAt(x, y);
    }

    fprintf(stderr, "\rfinished %0.2f%%", 100.0 * (x+1) / width);
  }

  saveToFile(imgDouble);
}

void alex::Camera::startRenderingOpenMP() const {
  cv::Mat imgDouble(cv::Size(width, height), CV_64FC3);

  #pragma omp parallel for schedule(dynamic, 1)       // OpenMP
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      imgDouble.at<cv::Vec3d>(y, x) = renderAt(x, y);
    }

    fprintf(stderr, "\rfinished %0.2f%%", 100.0 * (x+1) / width);
  }

  saveToFile(imgDouble);
}

void alex::Camera::renderThreads(size_t n) const {
  ThreadPool pool(n);

  int totalCount = width * height;

  std::shared_ptr<cv::Mat> imgDouble(new cv::Mat(cv::Size(width, height), CV_64FC3));
  std::shared_ptr<std::mutex> mu(new std::mutex);
  std::shared_ptr<int> finishedCount(new int(0));

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      pool.enqueue([x, y, totalCount, imgDouble, mu, finishedCount, this]() -> void {
        imgDouble->at<cv::Vec3d>(y, x) = renderAt(x, y);

        {
          std::lock_guard<std::mutex> _guard(*mu);
          (void)_guard;
          (*finishedCount)++;
          if (*finishedCount % (totalCount / 100) == 0)
            fprintf(stderr, "\rfinished %0.2f%%", 100.0 * (*finishedCount + 1) / totalCount);

          if (*finishedCount == totalCount) {
            saveToFile(*imgDouble);
            exit(0);
          }
        }

      });
    }
  }
}

void alex::Camera::saveToFile(const cv::Mat &imgDouble) const {
  cv::Mat img;
  imgDouble.convertTo(img, CV_8UC3, colorEnhancement * 255);
  imwrite("image.png", img);

  std::cout << imgDouble;
  serializeMat3d("tmp.mat", imgDouble);
}


std::shared_ptr<cv::Mat> alex::Camera::deserializeMat3d(std::string filePath) {
  return deserializeMat<double>(filePath);
}

template<class ElementT>
void alex::Camera::serializeMat(std::string filePath, const cv::Mat &mat) {
  std::ofstream of(filePath, std::ios::out | std::ios::trunc);
  of << mat.rows << ' ' << mat.cols << std::endl;
  for (int i = 0; i < mat.rows; ++i) {
    for (int j = 0; j < mat.cols; ++j) {
      auto vector = mat.at<cv::Vec<ElementT, 3>>(j, i);
      of << vector[0] << ' ' << vector[1] << ' ' << vector[2] << ' ';
    }
    of << std::endl;
  }
  of.close();
}

template<class ElementT>
std::shared_ptr<cv::Mat> alex::Camera::deserializeMat(std::string filePath) {
  std::ifstream ifs(filePath);
  int rows, cols;
  ifs >> rows >> cols;
  std::shared_ptr<cv::Mat> ret(new cv::Mat(cv::Size(cols, rows), CV_64FC3));
  for(int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      cv::Vec<ElementT, 3> a;
      ifs >> a[0] >> a[1] >> a[2];
      ret->at<cv::Vec<ElementT, 3>>(j, i) = a;
    }
  }
  return ret;
}

void alex::Camera::serializeMat3d(std::string filePath, const cv::Mat &mat) {
  serializeMat<double>(filePath, mat);
}



