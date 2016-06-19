//
// Created by alexwang on 6/19/16.
//

#include "Camera.h"
#include <cmath>

std::shared_ptr<Ray> alex::Camera::getRay(int x, int y) const {
  /* left = self.up.cross(self.frontN).normalize
  retina_center = self.position - self.frontN.normalize * self.image_distance
  retina_position = retina_center +
                    left * (2.0 * (x.to_f / self.width - 0.5) * self.retina_width) +
                    self.up.normalize * (2 * (y.to_f / self.height - 0.5) * self.retina_height)
  theta = Random.rand
  rand_vector = (left.normalize * Math.cos(theta) + self.up.normalize * Math.sin(theta)) * self.aperture_radius
  aperture_position = self.position + rand_vector
# direction = aperture_position - retina_position

  object_distance = self.focal_distance * self.image_distance / (self.image_distance - self.focal_distance)

# calculate focal plane
  point_on_focal_plane = self.position + (self.frontN.normalize) * object_distance
  normal_vector_focal_plane = self.frontN

# get the intersection of focal plane and this ray
  r = Ray.new(self.position - retina_position, retina_position)
                  target_point = intersect_plane(r, point_on_focal_plane, normal_vector_focal_plane)

  Ray.new(target_point - aperture_position, aperture_position) */

  if (x > width || y > height)
    return nullptr;

  auto retinaCenter = this->position - this->frontN * this->imageDistance;
  auto retinaPosition = retinaCenter +
                        leftN * ((2.0 * x / this->width - 1) * this->retinaWidth) +
                        upN * ((2.0 * y / this->height - 1) * this->retinaHeight);

  double theta = randRange(0, M_PI);
  auto randVec = (leftN * cos(theta) + upN * sin(theta)) * this->apertureRadius;
  auto aperturePosition = this->position + randVec;

  double objectDis = this->focalDistance * this->imageDistance / (this->imageDistance - this->focalDistance);
  auto pointOnFocalPlane = this->position + frontN * objectDis;
  auto normalVecFocalPlaneN = this->frontN;
  Ray r(retinaPosition, this->position - retinaPosition);
  auto targetPoint = intersectWithPlane(r, pointOnFocalPlane, normalVecFocalPlaneN);
  return std::shared_ptr<Ray>(new Ray(aperturePosition, targetPoint - aperturePosition));
}

cv::Vec3d alex::Camera::renderAt(int x, int y) const {
  
  return cv::Vec<double, 3>();
}

double alex::Camera::randRange(double start, double end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(start, end);
  return dis(gen);
}

cv::Vec3d alex::Camera::intersectWithPlane(const Ray &ray, const cv::Vec3d &point, const cv::Vec3d &normalVecN) {
  double t = (point - ray.getStartPoint()).dot(normalVecN) / (normalVecN.dot(ray.getDirectionN()));
  return ray.getStartPoint() + ray.getDirectionN() * t;
}







