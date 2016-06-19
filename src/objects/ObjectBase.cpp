//
// Created by alexwang on 6/19/16.
//

#include "ObjectBase.h"

bool alex::ObjectBase::reflect(const Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                               cv::Vec3d &color, Ray &outRay) const {
 double cosTheta = inRay.getDirectionN().dot(normalVecN);
 auto front = (normalVecN * (2 * cosTheta) + inRay.getDirectionN());
 outRay = Ray(intersection, front);
 color = reflectColor;
 return true;
}

bool alex::ObjectBase::diffuse(const alex::Ray &inRay, const cv::Vec3d &intersection, const cv::Vec3d &normalVecN,
                               cv::Vec3d &color, alex::Ray &outRay) const {
 return false;
}
