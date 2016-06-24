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

enum Mode {
  PathTrace,
  FromMatJSON
};

void pathTracing(int width, int monteCarloTimes, size_t threadCount, bool openmp);

void optParse(int argc, char **argv, Mode &mode, size_t &threadCount, bool &openmp, int &width, int &monteCarloTimes, double &rate, string &filePath) {

  if (strcmp(argv[1], "pt") == 0) {
    mode = Mode::PathTrace;
    if (argc != 5) {
      cout << "wrong parameters" << endl;
      exit(1);
    }
    openmp = strcmp(argv[2], "openmp") == 0;
    if (!openmp)
      threadCount = std::stoul(argv[2]);
    width = std::stoi(argv[3]);
    monteCarloTimes = std::stoi(argv[4]);
  }
  else if (strcmp(argv[1], "mat") == 0) {
    mode = Mode::FromMatJSON;
    if (argc != 4) {
      cout << "wrong parameters" << endl;
      exit(1);
    }
    filePath = argv[2];
    rate = std::stod(argv[3]);
  }
  else {
    cout << "wrong parameters" << endl;
    exit(1);
  }

}

int main(int argc, char** argv) {
  size_t threadCount;
  int width, monteCarloTimes;
  Mode mode;
  double rate;
  string filePath;
  bool opemmp;
  optParse(argc, argv, mode, threadCount, opemmp, width, monteCarloTimes, rate, filePath);

  srand(1);
  Logger::setup("pt.log", Logger::Level::Info);

  if (mode == Mode::PathTrace) {
    pathTracing(width, monteCarloTimes, threadCount, opemmp);
  }
  else if (mode == Mode::FromMatJSON) {
    Mat img;
    auto mat = Camera::deserializeMat3d(filePath);
    mat->convertTo(img, CV_8UC3, rate * 255);
    imwrite(filePath + ".png", img);
  }

  return 0;
}