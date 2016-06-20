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

void optParse(int argc, char **argv, size_t &threadCount, int &width, int &monteCarloTimes) {
  if (argc != 4) {
    cout << "wrong parameters" << endl;
    exit(1);
  }

  threadCount = std::stoul(argv[1]);
  width = std::stoi(argv[2]);
  monteCarloTimes = std::stoi(argv[3]);
}

int main(int argc, char** argv) {
  size_t threadCount;
  int width, height, monteCarloTimes;
  optParse(argc, argv, threadCount, width, monteCarloTimes);

  srand(1);
  Logger::setup("pt.log");

  height = width;

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
                monteCarloTimes,      // monte carlo times
                13                    // max trace depth
  );

  world->addObject(Sphere::createLight(
          "BigLight",
          Vec3d(8, 0, 4),  // position
          Vec3d(1, 0, 0),  // front
          Vec3d(0, 1, 0),  // up
          0.5,             // radius
          Vec3d(1, 1, 1)   // light color
  ));

  world->addObject(shared_ptr<Sphere>(new Sphere(
          "ReflectSphere",
          Vec3d(3, -1, 0),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0.7,            /* reflect */
          Vec3d(1, 1, 1),
          0.3,            /* diffuse */
          Vec3d(1, 1, 1),
          0,              /* refract */
          Vec3d(0, 0, 0),
          1,
          0.8
  )));

  world->addObject(shared_ptr<Sphere>(new Sphere(
          "RefractSphere",
          Vec3d(8, 1, 0),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0,                /* reflect */
          Vec3d(1, 1, 1),
          0.3,              /* diffuse */
          Vec3d(1, 1, 1),
          0.7,              /* refract */
          Vec3d(1, 1, 1),
          1.5,
          1.0               /* radius */
  )));

  world->addObject(shared_ptr<Sphere>(new Sphere(
          "SmallRefractSphere",
          Vec3d(8, 0, -2.5),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0,                /* reflect */
          Vec3d(1, 1, 1),
          0.3,              /* diffuse */
          Vec3d(1, 1, 1),
          0.7,              /* refract */
          Vec3d(1, 1, 1),
          1.5,
          0.4               /* radius */
  )));

  world->addObject(shared_ptr<Plane>(new Plane(
          "Ground",
          Vec3d(0, 0, -4),  // position
          Vec3d(0, 0, 1),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(1, 1, 1)
  )));

  world->addObject(shared_ptr<Plane>(new Plane(
          "Sky",
          Vec3d(0, 0, 4),  // position
          Vec3d(0, 0, -1),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(0, 0, 1)
  )));
  world->addObject(shared_ptr<Plane>(new Plane(
          "FrontWall",
          Vec3d(12, 0, 0),  // position
          Vec3d(-1, 0, 0),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(1, 1, 0)
  )));

  world->addObject(shared_ptr<Plane>(new Plane(
          "LeftWall",
          Vec3d(0, -4, 0), // position
          Vec3d(0, 1, 0),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(1, 0, 0)
  )));

  world->addObject(shared_ptr<Plane>(new Plane(
          "RightWall",
          Vec3d(0, 4, 0),  // position
          Vec3d(0, -1, 0), // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.5,             // reflect
          Vec3d(1, 1, 1),
          0.4,             // diffuse
          Vec3d(0, 1, 0)
  )));

  camera.startRendering(threadCount); // render threads
  return 0;
}