#include "src/Camera.h"
#include "src/World.h"
#include "src/objects/Sphere.h"
#include "src/utils/Logger.h"
#include "src/objects/Plane.h"
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>


using namespace std;
using namespace cv;
using namespace alex;

int main(int argc, char** argv) {
  srand(1);
  Logger::setup("pt.log");

  int width = 100;
  int height = width;

  double retinaWidth = 0.01;
  double retinaHeight = retinaWidth;

  auto world = shared_ptr<World>(new World);
  Camera camera(world,
                Vec3d(0, 0, 0),       // position
                Vec3d(1, 0, 0),       // front
                Vec3d(0, 0, 1),       // up
                retinaWidth,
                retinaHeight,
                width,
                height,
                1/(1/0.017 - 1/3.5),  // image distance
                0.0001,               // aperture
                0.017,                // f
                20                    // monte carlo times
  );

  auto bigLight = Sphere::createLight(
          "BigLight",
          Vec3d(4, 1, 0),  // position
          Vec3d(1, 0, 0),  // front
          Vec3d(0, 1, 0),  // up
          1,               // radius
          std::shared_ptr<const Vec3d>(new Vec3d(1, 1, 1)));

  world->addObject(bigLight);

  auto bigSphere = shared_ptr<Sphere>(new Sphere(
          "BigSphere",
          Vec3d(4, -1, 0),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0.5,
          Vec3d(1, 1, 1),
          0.4,
          Vec3d(1, 1, 1),
          1
  ));
  world->addObject(bigSphere);

  auto ground = shared_ptr<Plane>(new Plane(
          "Ground",
          Vec3d(0, 0, -1),  // position
          Vec3d(0, 0, 1),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(1, 1, 1)
  ));
  world->addObject(ground);

  camera.startRendering();
  return 0;
}