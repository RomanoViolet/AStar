/*
 * openNodes_UnitTest.cpp
 *
 *  Created on: Mar 16, 2018
 *      Author: rad2rng
 */

#include "OpenNode.hpp"

#include <cassert>


void openNode_UnitTest()
{
  Node a(3,4);
  float cost(10.56);

  OpenNode openNode(cost, a);

  //get the cost back
  assert(openNode.getCost() == cost && "OpenNode constructor failed");

  // get the node back
  assert(openNode.getNode() == a && "Node extraction (or Node comparison) test failed");

}
