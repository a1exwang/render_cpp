#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  Mat img(Size(320,240), CV_8UC3);
  imwrite("image.png", img);
  return 0;
}