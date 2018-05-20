/*
 * UnitTestsWrapper.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

extern void Node_UnitTest();
extern void CostGrid_UnitTest();
extern void Heuristics_UnitTest();
extern void openNode_UnitTest();
extern void AStar_UnitTest();

void performUnitTest()
{
  Node_UnitTest();
  CostGrid_UnitTest();
  Heuristics_UnitTest();
  openNode_UnitTest();
  AStar_UnitTest();
}


