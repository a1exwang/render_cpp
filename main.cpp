#include "src/Camera.h"
#include "src/World.h"
#include "src/objects/Sphere.h"
#include "src/utils/Logger.h"
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>


using namespace std;
using namespace cv;
using namespace alex;

int main(int argc, char** argv) {

  Logger::setup();

  Log.i("main", "hello\nmultiline");

  int width = 160;
  int height = 90;

  auto world = shared_ptr<World>(new World);


  auto bigLight = Sphere::createLight(
          "BigLight",
          Vec3d(3, 1, 5),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0.5,
          std::shared_ptr<const Vec3d>(new Vec3d(1, 1, 1)));

  auto bigSphere = shared_ptr<Sphere>(new Sphere(
          "BigSphere",
          Vec3d(4, 0, 0),
          Vec3d(1, 0, 0),
          Vec3d(0, 1, 0),
          0.5,
          Vec3d(1, 1, 1),
          0.4,
          Vec3d(1, 1, 1),
          0.5
  ));

  world->addObject(bigLight);
  world->addObject(bigSphere);


  Camera camera(world,
                Vec3d(0, 0, 0),
                Vec3d(1, 0, 0),
                Vec3d(0, 0, 1),
                1.6,
                0.9,
                width,
                height,
                0.01709,
                0.0001,
                0.0017
  );

  camera.startRendering();
  return 0;
}