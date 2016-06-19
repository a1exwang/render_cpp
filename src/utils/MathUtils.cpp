//
// Created by alexwang on 6/19/16.
//

#include "MathUtils.h"
#include <vector>
#include <random>

int alex::rouletteRandom(std::vector<double> arr) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dis(arr.begin(), arr.end());
  return dis(gen);
}
int alex::rouletteRandom(double *arr, int size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> dis(arr, arr + size);
  return dis(gen);
}

double alex::randRange(double start, double end) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(start, end);
  return dis(gen);
}