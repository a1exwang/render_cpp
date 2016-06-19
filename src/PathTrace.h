//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_PATHTRACE_H
#define RENDER_PATHTRACE_H

#include "utils/Ray.h"
#include "World.h"
#include <opencv/cv.h>

namespace alex {
class PathTrace {
public:
  PathTrace(const World &world) :world(world) { }
  cv::Vec3d trace(const Ray &ray) const;

private:
  static int rouletteRandom(std::vector<double> arr);
  static int rouletteRandom(double *arr, int size);

private:
  const World &world;
};
}



#endif //RENDER_PATHTRACE_H
