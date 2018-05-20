/*
 * Node_UnitTest.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#include "Node.hpp"
#include <cassert>
void
Node_UnitTest()
{
  // construction test
  Node a(2, 3);
  assert(a.get_X() == 2 && "Constructor Unit Test Failed");
  assert(a.get_Y() == 3 && "Constructor Unit Test Failed");

  // copy constructor test
  Node b = a;
  assert(b.get_X() == 2 && "Copy constructor Unit Test Failed");
  assert(b.get_Y() == 3 && "Copy constructor Unit Test Failed");

  // assignment test
  b = Node(5,6);
  assert(b.get_X() == 5 && "Assignment Operator Unit Test Failed");
  assert(b.get_Y() == 6 && "Assignment Operator Unit Test Failed");

  // comparison test
  a = b;
  assert(a == b && "Comparison Unit Test Failed");
}

