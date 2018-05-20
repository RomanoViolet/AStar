/*
 * SearchPath.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: dumbledore
 */
#include <cstdint>
#include "SearchPath.hpp"
#include "AStar.hpp"
#include <future>

/*
 * processes a given neighbor and adds it to the path option if the cheapest route to reach this neighbor was found.
 * @param: "cheapestNode" parent node around which to search the neighbor for.
 * @param: "thisNeighbor" the current neighbor being investigated
 * @param: "aStar" : the object used for searching. Required since processThisNeighbor is not part of the AStar class
 * @param: "weight" optimality to speed tradeoff ration. Higher weight will reduce compute time at the cost of inefficient path
 * @param: "end": the final target of the path.
 */
void processThisNeighor(Node cheapestNode, Node thisNeighbor, Algo::AStar& aStar, const float weight, const Node end)
{

  // temporaries
  float newGScore = 0.0f;
  float newFScore = 0.0f;


  // new G Score: Cost upto <cheapestNode>  +  Cost to Move to the <thisNeighbor> assuming flat terrain  +  Cost due to difference in terrain
  newGScore =
      aStar.getCostAt(cheapestNode)
      + aStar.getCostHeuristic(cheapestNode, thisNeighbor)
      + (aStar.getTerrainAt(thisNeighbor)
          - aStar.getTerrainAt(cheapestNode));

  // Is the newGScore to reach thisNeighbor greater than what has already been seen (via another path)?
  if (newGScore < aStar.getCostAt(thisNeighbor)) {
    // Yes. This path is the cheapest.

    // revise the recorded g-score
    aStar.recordCostAt(thisNeighbor, newGScore);

    // also record the path
    aStar.recordPath(thisNeighbor, cheapestNode);

    // compute the f-score for thisNeighbor
    newFScore = aStar.getCostAt(thisNeighbor) + (weight * aStar.getCostHeuristic(thisNeighbor, end));

    // add thisNeighbor to openList
    if(! aStar.isNodeInOpenSet(thisNeighbor))
    {
      aStar.insertNodeInOpenSet(newFScore, thisNeighbor);
    }


  } else {
    // skip this neighbor

    // add this node to the closed set
    aStar.insertNodeInClosedSet(thisNeighbor);

    /*continue;*/
  }
}

void searchPath(const Node start, const Node end,
                const unsigned image_XSize,
                const unsigned image_YSize,
                const std::vector<uint8_t> &elevationData,
                const std::vector<uint8_t> &overridesData,
                const uint8_t mask,
                const float weight,
                             const std::vector<std::pair<int, int>>& moves, std::vector<Node>& path, float& totalDistanceTravelled, float& totalTimeOfTravel) {

  // instantiate the heuristic
  Heuristics h;

  // instantiate the AStar solver
  Algo::AStar aStar(
      std::bind(&Heuristics::EuclideanDistance, &h, std::placeholders::_1,
                std::placeholders::_2),
      image_XSize, image_YSize, moves);

  // set the mask to indicate restricted areas
  aStar.setMask(mask);

  // build terrain map
  aStar.buildTerrainMap(elevationData, overridesData);

  // initialize the movement cost (g)
  aStar.recordCostAt(start, aStar.getTerrainAt(start));

  // add the start point to the open list
  aStar.insertNodeInOpenSet(aStar.getTerrainAt(start), start);

  // add the start node to the already explored list
  aStar.insertNodeInClosedSet(start);

  // explore

  // temporaries
  Node cheapestNode(0, 0);
  std::vector<Node> allNeighbors;


  while (!aStar.isOpenSetEmpty()) {
    // find the cheapest node in the open set. This will also remove this node from the set.
    cheapestNode = aStar.getCheapestNodeInOpenSet();

    // add this node to the closed set
    aStar.insertNodeInClosedSet(cheapestNode);

    // Is this the end node?
    if (cheapestNode == end) {
      // done with search. Return path.
      break;
    }

    // get a list of all unexplored neighbors of cheapestNode
    allNeighbors = aStar.getExplorableNeighborsOf(cheapestNode);

    // For each explorable neighbor
    for (Node thisNeighbor : allNeighbors) {
 

     std::future<void> t = std::async(std::launch::async, processThisNeighor, cheapestNode, thisNeighbor, std::ref(aStar), weight, end);
 
    }  //for (std::vector<Node> thisNeighbor : allNeighbors)


  } //while(!aStar.isOpenSetEmpty())

  path = aStar.finalPath(start, end);
  assert(path.size() > 1 && "Failed to find the path");

  // compute distance travelled and time required.
  totalDistanceTravelled = 0.0f;
  totalTimeOfTravel = 0.0f;
  aStar.computeDistanceAndTimeMetrics(path, totalDistanceTravelled, totalTimeOfTravel);
}
