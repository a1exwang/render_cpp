//
// Created by alexwang on 6/19/16.
//

#include "PathTrace.h"
#include "utils/Logger.h"
#include "utils/MathUtils.h"

cv::Vec3d alex::PathTrace::trace(const alex::Ray &ray, TraceInfo *info) const {
  return doTrace(ray, info, cv::Vec3d(1, 1, 1));
}

cv::Vec3d alex::PathTrace::doTrace(const Ray &ray, TraceInfo *info, const cv::Vec3d &prevColor) const {
  if (norm(prevColor) < alex::Epsilon || info->data.size() > 10) {
    info->appendInfo(PT_TYPE_TOO_WEAK);
    Log.i("trace", info->toString());
    return prevColor;
  }

  cv::Vec3d intersection, normalVecN;
  bool outsideIn;
  auto object = world.determineIntersection(ray, intersection, normalVecN, outsideIn);

  if (object != nullptr) {
    if (object->isALight()) {
      auto lightColor = object->getLightColor();
      if (lightColor != nullptr) {
        info->appendInfo(PT_TYPE_LIGHT, object, intersection);
        Log.i("trace", info->toString());
        return *lightColor;
      }
      else {
        std::cout << "light(" << object->getName() << ") without a color" << std::endl;
        throw 1;
      }
    }
    else {
//      double probabilities[] = { object->getDiffuseProbability(), object->getReflectProbability() };
      std::initializer_list<double> list({ object->getDiffuseProbability(), object->getReflectProbability() });
      constexpr int indexDiffuse = 0, indexReflect = 1;
      int item = rouletteRandom(list);
      cv::Vec3d color;
      Ray outRay;
      if (item == indexDiffuse && object->diffuse(ray, intersection, normalVecN, color, outRay)) {
        info->appendInfo(PT_TYPE_DIFFUSE, object, intersection, outRay.getDirectionN());
        return doTrace(outRay, info, prevColor.mul(color));
      }
      else if (item == indexReflect && object->reflect(ray, intersection, normalVecN, color, outRay)) {
        info->appendInfo(PT_TYPE_REFLECT, object, intersection, outRay.getDirectionN());
        return doTrace(outRay, info, prevColor.mul(color));
      }
      else if (item >= (int)list.size()) {
        Log.e("trace", "invalid rouletteRandom");
        throw 1;
      }
      else {
        info->appendInfo(PT_TYPE_NO_OUTBOUND);
        Log.i("trace", info->toString());
        return cv::Vec3d(0, 0, 0);
      }
    }
  }
  else {
    info->appendInfo(PT_TYPE_NO_INTERSECTION);
    Log.v("trace", info->toString());
    return cv::Vec3d(0, 0, 0);
  }
}



