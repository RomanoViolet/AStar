/*
 * AStar_UnitTest.cpp
 *
 *  Created on: Mar 16, 2018
 *      Author: dumbledores
 */
#include <cmath>
#include <limits>
#include <cassert>
#include <algorithm>
#include "AStar.hpp"
#include "Heuristics.hpp"

void AStar_UnitTest() {
  // construction test
  Heuristics h;
  unsigned image_XSize = 2;
  unsigned image_YSize = 3;
  std::vector<std::pair<int, int>> moves = { std::make_pair(1, 0),
      std::make_pair(0, -1), std::make_pair(0, 1),
      std::make_pair(-1, 0), std::make_pair(-1, -1), std::make_pair(1, -1),
      std::make_pair(1, 1), std::make_pair(-1, 1) };
  //auto fptr = std::bind(&Heuristics::EuclideanDistance, &h, std::placeholders::_1, std::placeholders::_2);

  Algo::AStar aStar(
      std::bind(&Heuristics::EuclideanDistance, &h, std::placeholders::_1,
                std::placeholders::_2),
      image_XSize, image_YSize, moves);

  // We expect the map to be of size 2x3
  unsigned x;
  unsigned y;
  aStar.getMapDimensions(x, y);
  assert(
      ((x == 2) && (y == 3))
          && "Constructor for AStar Failed: Image Dimensions Not Set Properly");

  // Build the cost map
  std::vector<uint8_t> elevationData = { 1, 2, 3, 4, 5, 6 };
  std::vector<uint8_t> overridesData = { 0x60, 0x50, 0x40, 0x30, 0x20, 0x10 };

  /*
   * Elevation Data layout
   * 1 2
   * 3 4
   * 5 6
   *
   * Override Data Layout
   * 0x60 0x50
   * 0x40 0x30
   * 0x20 0x10
   */
  // set the mask
  aStar.setMask(0x10); // only OF_RIVER_MARSH

  // build cost map
  aStar.buildTerrainMap(elevationData, overridesData);

  // retrieve cost at location (x=0, y=0), We expect the cost to be 1 since bits 5 and 6 are set
  assert(
      aStar.getTerrainAt(Node(0, 0)) == 1 && "Parsing Overrides Data Failed");

  // retrieve cost at location (x=1, y=0)
  assert(
      aStar.getTerrainAt(Node(1, 0)) == 2 && "Parsing Overrides Data Failed");

  // retrieve cost at location (x=0, y=1), We expect the cost to be 3 since bits 6 is set
  assert(
      aStar.getTerrainAt(Node(0, 1)) == 3 && "Parsing Overrides Data Failed");

  // retrieve cost at location (x=1, y=1), We expect the cost to be 3 since bits 4 and 4 are set
  assert(
      aStar.getTerrainAt(Node(1, 1)) == 4 && "Parsing Overrides Data Failed");

  // retrieve cost at location (x=0, y=2). We expect the cost to be 5
  assert(
      aStar.getTerrainAt(Node(0, 2)) == 5 && "Parsing Elevation Data Failed");

  // retrieve cost at location (x=1, y=2), We expect the cost to be 3 since bits 4 is set
  assert(
      aStar.getTerrainAt(Node(1, 2)) == 6 && "Parsing Overrides Data Failed");


  // retrieve overrides: Expect an override at (x=1, y=2)
  assert(
        aStar.getOverrideAt(Node(1, 2)) && "Parsing Overrides Data Failed");

  // retrieve overrides: Expect an override at (x=1, y=1)
  assert(
        aStar.getOverrideAt(Node(1, 1)) && "Parsing Overrides Data Failed");


  // Test the collection methods
  assert(
      aStar.isOpenSetEmpty() == true
          && "Collection Method 'isCollectionEmpty' failed");

  // Test the collection methods
  assert(
      aStar.isNodeInOpenSet(Node(2, 3)) == false
          && "Collection Method 'isNodeInCollection' failed");

  // Test the collection methods
  // insert a new node
  aStar.insertNodeInOpenSet(4.55f, Node(2, 3));

  // see whether this node is now in the collection
  assert(
      aStar.isNodeInOpenSet(Node(2, 3)) == true
          && "Collection Method 'isNodeInCollection' failed");

  // get the cheapest node. It should ne Node(2,3)
  Node res = aStar.getCheapestNodeInOpenSet();
  assert(res == Node(2, 3) && "Collection Method 'getCheapestNode' failed");

  // the collection should now be empty
  // Test the collection methods
  assert(
      aStar.isNodeInOpenSet(Node(2, 3)) == false
          && "Collection Method 'isNodeInCollection' failed");

  assert(
      aStar.isOpenSetEmpty() == true
          && "Collection Method 'isCollectionEmpty' failed");

  // insert a nodes
  aStar.insertNodeInOpenSet(4.55f, Node(2, 3));
  aStar.insertNodeInOpenSet(0.15f, Node(3, 3));

  // get the cheapest node. It should ne Node(2,3)
  assert(
      aStar.getCheapestNodeInOpenSet() == Node(3, 3)
          && "Collection Method 'getCheapestNode' failed");

  // see whether the collection is not empty
  assert(
      aStar.isOpenSetEmpty() == false
          && "Collection Method 'isCollectionEmpty' failed");

  // get the cheapest node. It should ne Node(2,3)
  assert(
      aStar.getCheapestNodeInOpenSet() == Node(2, 3)
          && "Collection Method 'getCheapestNode' failed");

  // see whether the collection is empty
  assert(
      aStar.isOpenSetEmpty() == true
          && "Collection Method 'isCollectionEmpty' failed");

  // test the use of heuristic function
  Node a(2, 4);
  Node b(0, 8);
  assert(
      (aStar.getCostHeuristic(a, b) - std::pow(20.0, 0.5))
          < std::numeric_limits<float>::epsilon());

  // test closedList. Node (1,2) is forbidden with an override flag, and is therefore closed.
  assert(
      (aStar.isNodeInClosedSet(Node(1, 2)) == true)
          && "ClosedList Test Failed");

  // insert a node in the closed list
  aStar.insertNodeInClosedSet(Node(1, 2));

  // test closedList
  assert(
      (aStar.isNodeInClosedSet(Node(1, 2)) == true)
          && "ClosedList Test Failed");

  // test closedList
  assert(
      (aStar.isNodeInClosedSet(Node(0, 0)) == false)
          && "ClosedList Test Failed");

  // test costs: Initially all nodes are set to infinity
  assert(
      std::isinf(aStar.getCostAt(Node(1, 2))) && "Cost Method getCostAt failed");

  // record a cost
  aStar.recordCostAt(Node(1, 2), 1.45);

  // test costs
  assert(
      ((aStar.getCostAt(Node(1, 2)) - 1.45)
          < std::numeric_limits<float>::epsilon())
          && "Cost Method getCostAt failed");

  // test neighbor computation
  Node n(0, 1);
std::vector<Node> allNeighbors = aStar.getExplorableNeighborsOf(n);

// Expected result: Node(0,0), Node(0,2)

// The number of neighbors are expected to be 2
assert(allNeighbors.size() == 2 && "Neighbor Computation Failed");


// confirm Node(0,0) is allNeighbors
std::vector<Node>::iterator it = std::find(allNeighbors.begin(), allNeighbors.end(), Node(0,0));
assert(it != allNeighbors.end() && "Neighbor Computation Failed");





// remove this neighbor
allNeighbors.erase(it);

// confirm Node(0,2) is allNeighbors
it = std::find(allNeighbors.begin(), allNeighbors.end(), Node(0,2));
assert(it != allNeighbors.end() && "Neighbor Computation Failed");


}

