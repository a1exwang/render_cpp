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

  int width = 80;
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
                20,                   // monte carlo times
                13                    // max trace depth
  );

  auto bigLight = Sphere::createLight(
          "BigLight",
          Vec3d(8, 0, 4),  // position
          Vec3d(1, 0, 0),  // front
          Vec3d(0, 1, 0),  // up
          0.5,             // radius
          Vec3d(1, 1, 1)   // light color
  );

  world->addObject(bigLight);

  auto bigSphere = shared_ptr<Sphere>(new Sphere(
          "BigSphere",
          Vec3d(3, -1, 0),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0.5,            /* reflect */
          Vec3d(1, 1, 1),
          0.4,            /* diffuse */
          Vec3d(0, 1, 0),
          0.3
  ));
  world->addObject(bigSphere);

  auto ground = shared_ptr<Plane>(new Plane(
          "Ground",
          Vec3d(0, 0, -4),  // position
          Vec3d(0, 0, 1),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(1, 1, 1)
  ));
  world->addObject(ground);

  auto sky = shared_ptr<Plane>(new Plane(
          "Sky",
          Vec3d(0, 0, 4),  // position
          Vec3d(0, 0, -1),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(0, 0, 1)
  ));
  world->addObject(sky);

  auto front = shared_ptr<Plane>(new Plane(
          "FrontWall",
          Vec3d(14, 0, 0),  // position
          Vec3d(-1, 0, 0),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(1, 1, 0)
  ));
  world->addObject(front);

  auto left = shared_ptr<Plane>(new Plane(
          "LeftWall",
          Vec3d(0, -4, 0),  // position
          Vec3d(0, 1, 0),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(1, 0, 0)
  ));
  world->addObject(left);

  auto right = shared_ptr<Plane>(new Plane(
          "LeftWall",
          Vec3d(0, 4, 0),  // position
          Vec3d(0, -1, 0),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(0, 1, 0)
  ));
  world->addObject(right);

  camera.startRendering(4);
  return 0;
}