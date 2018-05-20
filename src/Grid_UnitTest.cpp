/*
 * Node_UnitTest.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#include "Grid.hpp"

#include <cassert>
#include <limits>
void
Grid_UnitTest()
{
  // construction test
  Grid c(2, 3);
  assert(c(1, 2) == 0.0f && "Constructor Unit Test Failed");

  c.clear();

  // assignment test
  c(1, 2) = 4;
  assert(c(1, 2) == 4 && "image pixel assignment Unit Test Failed");

  //set to infinity
  c.makeAllInfinity();
  assert(c(1, 0) > std::numeric_limits<float>::max() && "image makeAllInfinity Unit Test Failed");

  // clear test
  c.clear();
  assert(c(1, 2) == 0.0f && "Clear Unit Test Failed");


}

