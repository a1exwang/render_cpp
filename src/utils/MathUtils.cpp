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
int alex::rouletteRandom(const std::initializer_list<double> &list) {
  double sum = 0;
  for (auto value : list) {
    sum += value;
  }
  double r = randRange(0, sum);
  double acc = 0;
  int i = 0;
  for (auto value : list) {
    if (acc <= r && r < acc + value) {
      return i;
    }
    acc += value;
    i++;
  }
  return (int)list.size() - 1;

//  std::random_device rd;
//  std::mt19937 gen(rd());
//  std::discrete_distribution<> dis(list);
//  return dis(gen);
}


double alex::randRange(double start, double end) {
  return (double)rand() / RAND_MAX * (end - start) + start;

//  std::random_device rd;
//  std::mt19937 gen(rd());
//  std::uniform_real_distribution<> dis(start, end);
//  return dis(gen);
}