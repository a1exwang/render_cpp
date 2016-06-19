//
// Created by alexwang on 6/19/16.
//

#include "PathTrace.h"
#include "utils/Logger.h"

cv::Vec3d alex::PathTrace::trace(const alex::Ray &ray) const {
  cv::Vec3d intersection, normalVecN;
  bool outsideIn;
  auto object = world.determineIntersection(ray, intersection, normalVecN, outsideIn);

  if (object != nullptr) {
    Log.i("trace", "intersect(%s)", 0, object->getName().c_str());
    if (object->isALight()) {
      auto lightColor = object->getLightColor();
      if (lightColor != nullptr) {
        return *lightColor;
      }
      else {
        std::cout << "light(" << object->getName() << ") without a color" << std::endl;
        throw 1;
      }
    }
    else {
      double probabilities[] = { object->getDiffuseProbability(), object->getReflectProbability() };
      int item = rouletteRandom(probabilities, sizeof(probabilities));
      cv::Vec3d color;
      Ray outRay;
      if (item == 0 && object->diffuse(ray, intersection, normalVecN, color, outRay)) {
        Log.i("trace", "diffuse(%s)", 0, object->getName().c_str());
        return trace(outRay).mul(color);
      }
      else if (item == 1 && object->reflect(ray, intersection, normalVecN, color, outRay)) {
        Log.i("trace", "reflect(%s)", 0, object->getName().c_str());
        return trace(outRay).mul(color);
      }
    }
  }
  return cv::Vec3d(0, 0, 0);
}

int alex::PathTrace::rouletteRandom(std::vector<double> arr) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dis(arr.begin(), arr.end());
  return dis(gen);
}
int alex::PathTrace::rouletteRandom(double *arr, int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dis(arr, arr + size);
  return dis(gen);
}