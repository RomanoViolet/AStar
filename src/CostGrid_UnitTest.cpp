/*
 * Node_UnitTest.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#include "CostGrid.hpp"
#include <cassert>
void CostGrid_UnitTest() {
  // construction test
  CostGrid<unsigned> c(2, 3);
  assert(c(1, 2) == 0 && "Constructor Unit Test Failed");

  c.clear();

  // assignment test
  c(1, 2) = 4;
  assert(c(1, 2) == 4 && "image pixel assignment Unit Test Failed");

  //set to infinity
  c.makeAllInfinity();
  if (std::numeric_limits<unsigned>::has_infinity) {
    assert(
        c(1, 0) > std::numeric_limits<unsigned>::max()
        && "image makeAllInfinity Unit Test Failed");
  } else {

    assert(
        c(1, 0) == std::numeric_limits<unsigned>::max()
        && "image makeAllInfinity Unit Test Failed");
  }

  // clear test
  c.clear();
  assert(c(1, 2) == 0.0f && "Clear Unit Test Failed");

  // construction test for float types
  CostGrid<float> d(2, 3);
  assert(d(1, 2) == 0.0f && "Constructor Unit Test Failed");

  d(1, 2) = 4.45;
  std::fabs( ( d(1, 2) - 4.45 )  <   std::numeric_limits<float>::epsilon() && "image pixel assignment Unit Test Failed");

  //set to infinity
  d.makeAllInfinity();
  if (std::numeric_limits<float>::has_infinity) {
    assert(
        d(1, 0) > std::numeric_limits<float>::max()
        && "image makeAllInfinity Unit Test Failed");
  } else {

    assert(
        d(1, 0) == std::numeric_limits<float>::max()
        && "image makeAllInfinity Unit Test Failed");
  }

}

