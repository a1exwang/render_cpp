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

void pathTracing(int width, int monteCarloTimes, size_t threadCount, bool opemmp) {

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
                monteCarloTimes,      // monte carlo times
                20,                   // max trace depth
                1.5                    // color enhancement
  );

  world->addObject(Sphere::createLight(
          "BigLight",
          "steel.binary",
          Vec3d(8, 0, 4),  // position
          Vec3d(1, 0, 0),  // front
          Vec3d(0, 1, 0),  // up
          1,             // radius
          Vec3d(1, 1, 1)   // light color
  ));

  world->addObject(new Sphere(
          "ReflectSphere",
          "steel.binary",
          Vec3d(9, -3.5, -3),
          Vec3d(1, 0, 0),
          Vec3d(1, 1, 1),
          0.1,            /* reflect */
          Vec3d(1, 1, 1),
          0.9,            /* diffuse */
          Vec3d(1, 1, 0),
          0,              /* refract */
          Vec3d(0, 0, 0),
          0.5,
          1
  ));
  world->addObject(new Sphere(
          "DiffuseSphere",
          "steel.binary",
          Vec3d(6, -2.4, -3.4),
          Vec3d(1, 0, 0),
          Vec3d(1, 1, 1),
          0.1,            /* reflect */
          Vec3d(1, 1, 1),
          0.9,            /* diffuse */
          Vec3d(1, 1, 1),
          0,              /* refract */
          Vec3d(0, 0, 0),
          0.5,
          0.6
  ));
  world->addObject(new Sphere(
          "RefractSphere",
          "steel.binary",
          Vec3d(3, 0.5, 0),
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
  ));

  world->addObject(new Sphere(
          "SmallRefractSphere",
          "steel.binary",
          Vec3d(8, 2, -2),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0,                /* reflect */
          Vec3d(1, 1, 1),
          0,              /* diffuse */
          Vec3d(1, 1, 1),
          1,              /* refract */
          Vec3d(0.9, 1, 1),
          1.5,
          0.4               /* radius */
  ));

  world->addObject(new Plane(
          "Floor",
          "green-plastic.binary",
          Vec3d(0, 0, -4),  // position
          Vec3d(0, 0, 1),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.1,             // reflect
          Vec3d(0.9, 0.9, 0.9),
          0.9,             // diffuse
          Vec3d(1, 1, 1)
  ));

  world->addObject(new Plane(
          "Ceil",
          "green-plastic.binary",
          Vec3d(0, 0, 4),  // position
          Vec3d(0, 0, -1),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.1,             // reflect
          Vec3d(0.9, 0.9, 0.9),
          0.9,             // diffuse
          Vec3d(1, 1, 1)
  ));
  world->addObject(new Plane(
          "FrontWall",
          "green-plastic.binary",
          Vec3d(12, 0, 0),  // position
          Vec3d(-1, 0, 0),  // normalVec
          Vec3d(0, 1, 0),  // yAxis
          0.1,             // reflect
          Vec3d(0.8, 0.8, 0.8),
          0.9,             // diffuse
          Vec3d(1, 1, 1)
  ));
//  world->addObject(shared_ptr<Plane>(new Plane(
//          "BackWall",
//          Vec3d(-1, 0, 0),  // position
//          Vec3d(1, 0, 0),  // normalVec
//          Vec3d(0, 1, 0),  // yAxis
//          1,             // reflect
//          Vec3d(1, 1, 1),
//          0,             // diffuse
//          Vec3d(1, 1, 1)
//  )));
  world->addObject(new Plane(
          "LeftWall",
          "green-plastic.binary",
          Vec3d(0, -4, 0), // position
          Vec3d(0, 1, 0),  // normalVec
          Vec3d(0, 0, 1),  // yAxis
          0.1,             // reflect
          Vec3d(0.4, 0.4, 1),
          0.9,             // diffuse
          Vec3d(0.4, 0.4, 1)
  ));

  world->addObject(new Plane(
          "RightWall",
          "green-plastic.binary",
          Vec3d(0, 4, 0),  // position
          Vec3d(0, -1, 0), // normalVec
          Vec3d(0, 0, 1),  // yAxis
          0.1,             // reflect
          Vec3d(0.4, 1, 0.4),
          0.9,             // diffuse
          Vec3d(0.4, 1, 0.4)
  ));

  if (opemmp) {
    camera.startRenderingOpenMP();
  }
  else {
    camera.startRendering(threadCount);
  }
}
