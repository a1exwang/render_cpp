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
  if (/*norm(prevColor) < alex::Epsilon * 0.1 ||*/ info->data.size() > maxTraceDepth) {
    info->appendInfo(PT_TYPE_TOO_WEAK, prevColor);
    Log.v("trace", info->toString());
    return prevColor;
  }

  cv::Vec3d intersection, normalVecN;
  bool outsideIn;
  auto object = world.determineIntersection(ray, intersection, normalVecN, outsideIn);

  if (object != nullptr) {
    if (object->isALight()) {
      auto lightColor = object->getLightColor();
      if (lightColor != nullptr) {
        info->appendInfo(PT_TYPE_LIGHT, prevColor, object, intersection, cv::Vec3d(), outsideIn);
        Log.i("trace", info->toString());
        return *lightColor;
      }
      else {
        std::cout << "light(" << object->getName() << ") without a color" << std::endl;
        throw 1;
      }
    }
    else {
//      cv::Vec3d color;
//      Ray outRay;
//      if (object->brdf(ray, intersection, normalVecN, color, outRay)) {
//        info->appendInfo(PT_TYPE_BRDF, prevColor, object, intersection, outRay.getDirectionN(), outsideIn);
//        return doTrace(outRay, info, prevColor.mul(color));
//      }
//      else {
//        throw "brdf failed";
//      }
      std::initializer_list<double> list(
              { object->getDiffuseProbability(), object->getReflectProbability(), /*object->getRefractProbability()*/ });
      constexpr int indexDiffuse = 0, indexReflect = 1, indexRefract = 2;
      int item = rouletteRandom(list);
      cv::Vec3d color;
      Ray outRay;
      if (item == indexDiffuse && object->diffuse(ray, intersection, normalVecN, color, outRay)) {
        info->appendInfo(PT_TYPE_DIFFUSE, prevColor, object, intersection, outRay.getDirectionN(), outsideIn);
        return doTrace(outRay, info, prevColor.mul(color));
      }
      else if (item == indexReflect && object->reflect(ray, intersection, normalVecN, color, outRay)) {
        if (object->getName() == "FrontWall" && info->data.size() == 0) {
          info->appendInfo(PT_TYPE_REFLECT, prevColor, object, intersection, outRay.getDirectionN(), outsideIn);
          return doTrace(outRay, info, prevColor.mul(color));
        }

        info->appendInfo(PT_TYPE_REFLECT, prevColor, object, intersection, outRay.getDirectionN(), outsideIn);
        return doTrace(outRay, info, prevColor.mul(color));
      }
      else if (item == indexRefract && object->refract(ray, intersection, normalVecN, color, outRay)) {
        info->appendInfo(PT_TYPE_REFRACT, prevColor, object, intersection, outRay.getDirectionN(), outsideIn);
        return doTrace(outRay, info, prevColor.mul(color));
      }
      else if (item >= (int)list.size()) {
        Log.e("trace", "invalid rouletteRandom");
        throw 1;
      }
      else {
        info->appendInfo(PT_TYPE_NO_OUTBOUND, prevColor);
        Log.i("trace", info->toString());
        return cv::Vec3d(0, 0, 0);
      }
    }
  }
  else {
    info->appendInfo(PT_TYPE_NO_INTERSECTION, prevColor);
    Log.v("trace", info->toString());
    return cv::Vec3d(0, 0, 0);
  }
}



