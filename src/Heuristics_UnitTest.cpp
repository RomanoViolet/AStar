/*
 * Node_UnitTest.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#include "Heuristics.hpp"
#include <cassert>
#include <cmath>
#include <limits>
void Heuristics_UnitTest() {
  // Euclidean Distance Test
  Node a(2, 3);
  Node b(4, 5);
  Heuristics heuristics;
  assert(
      std::fabs(heuristics.EuclideanDistance(a, b) - std::pow(8.0, 0.5))
          < std::numeric_limits<float>::epsilon());

  // Euclidean Distance Test
  a = {2, 7};
  b = {4, 0};

  assert(
      std::fabs(heuristics.EuclideanDistance(a, b) - std::pow(53.0, 0.5))
          < std::numeric_limits<float>::epsilon());

}

