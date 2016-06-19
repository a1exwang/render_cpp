#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>
#include <initializer_list>

namespace alex {

int rouletteRandom(std::vector<double> arr);
int rouletteRandom(const std::initializer_list<double> &list);
double randRange(double start, double end);

}

#endif // MATH_UTILS_H
