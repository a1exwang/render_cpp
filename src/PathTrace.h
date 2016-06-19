//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_PATHTRACE_H
#define RENDER_PATHTRACE_H

#include "helpers/Ray.h"
#include "World.h"
#include <opencv/cv.h>

namespace alex {
class PathTrace {
public:
  PathTrace(const World &world) :world(world) { }
private:
  cv::Vec3d trace(const Ray &ray) const;

  const World &world;
};
}



#endif //RENDER_PATHTRACE_H
