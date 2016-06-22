//
// Created by alexwang on 6/20/16.
//

#ifndef RENDER_BRDF_H
#define RENDER_BRDF_H

#include <string>
#include <cxcore.h>
#include <memory>

namespace alex {
class BRDF {
public:
  BRDF(std::string fileName);
  void readColor(double thetaIn, double phiIn, double thetaOut, double phiOut, cv::Vec3d &color);
private:
  std::shared_ptr<double> brdf;
};
}


#endif //RENDER_BRDF_H
