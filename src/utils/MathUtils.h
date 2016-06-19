#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>

namespace alex {

int rouletteRandom(std::vector<double> arr);
int rouletteRandom(double *arr, int size);
double randRange(double start, double end);

}

#endif // MATH_UTILS_H
