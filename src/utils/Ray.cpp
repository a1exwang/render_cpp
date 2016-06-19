//
// Created by alexwang on 6/19/16.
//

#include "Ray.h"
#include <sstream>

std::string alex::Ray::toString() {
  std::stringstream ss;
  ss << "ray(" << startPoint << " " << directionN << ")";
  return ss.str();
}

