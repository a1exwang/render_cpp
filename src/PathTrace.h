//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_PATHTRACE_H
#define RENDER_PATHTRACE_H

#include "utils/Ray.h"
#include "World.h"
#include "utils/Logger.h"
#include <opencv/cv.h>

namespace alex {

constexpr const char *PT_TYPE_DIFFUSE = "diffuse";
constexpr const char *PT_TYPE_BRDF = "brdf";
constexpr const char *PT_TYPE_REFLECT = "reflect";
constexpr const char *PT_TYPE_REFRACT = "refract";
constexpr const char *PT_TYPE_LIGHT = "light";
constexpr const char *PT_TYPE_NO_OUTBOUND = "dead";
constexpr const char *PT_TYPE_NO_INTERSECTION = "noIntersection";
constexpr const char *PT_TYPE_ROOT = "root";
constexpr const char *PT_TYPE_TOO_WEAK = "too_weak";

class PathTrace {
public:
  struct TraceInfo {
    int x, y;
    size_t depth;
    struct _Data {
      const ObjectBase *object;
      cv::Vec3d prevColor;
      std::string type;
      cv::Vec3d intersection, direction;
      bool outsideIn;
    };
    std::vector<_Data> data;

    TraceInfo(int x, int y) :x(x), y(y), depth(0) { }

    void appendInfo(std::string type, const cv::Vec3d& prevColor, const ObjectBase *object = nullptr,
                    cv::Vec3d intersection = cv::Vec3d(),
                    cv::Vec3d direction = cv::Vec3d(), bool outsideIn = true) {
      _Data d;
      d.type = type;
      d.prevColor = prevColor;
      d.object = object;
      d.intersection = intersection;
      d.direction = direction;
      d.outsideIn = outsideIn;
      data.push_back(d);
    }

    std::string toString() const {
      std::stringstream ss;
      ss << "At (" << x << ", " << y << ")" << std::endl;

      for (std::size_t depth = 0; depth < data.size(); ++depth) {
        auto item = data[depth];

        // first line
        ss << item.type << ", depth = " << depth << std::endl;
        ss << "  prevColor " << item.prevColor << std::endl;
        // rest lines
        if (item.type == PT_TYPE_DIFFUSE || item.type == PT_TYPE_REFLECT || item.type == PT_TYPE_LIGHT) {
          ss << "  on object(" << item.object->getName() << ", " << vec3ToStr(item.intersection) << ")" << std::endl;
          ss << "  direction " <<  vec3ToStr(item.direction) << std::endl;
          ss << "  " << (item.outsideIn ? "outsideIn" : "insideOut") << std::endl;
        }
      }
      ss << std::endl;

      return ss.str();
    }
  };
  PathTrace(const World &world, size_t maxTraceDepth) :world(world), maxTraceDepth(maxTraceDepth) { }
  cv::Vec3d trace(const Ray &ray, TraceInfo *info) const;

private:
  cv::Vec3d doTrace(const Ray &ray, TraceInfo *info, const cv::Vec3d &prevColor) const;

  const World &world;
  size_t maxTraceDepth;
};
}



#endif //RENDER_PATHTRACE_H
