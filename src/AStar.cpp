/*
 * AStar.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */
#include <cmath>
#include "AStar.hpp"

namespace Algo {

inline AStar::~AStar() {
}

void AStar::buildTerrainMap(const std::vector<uint8_t>& elevationData,
                            const std::vector<uint8_t>& overridesData) {
  // fill the map
  for (unsigned y = 0; y < this->image_YSize; ++y) {
    for (unsigned x = 0; x < this->image_XSize; ++x) {
      // if the (x,y) location is a marsh or a river, then we raise the cost of this location to the maximum possible.
      unsigned index = y * this->image_XSize + x;
      this->terrainMap(x, y) = elevationData[index];

      // if the value at location (x,y) is 1, then that location is not available for exploration
      if((overridesData[index] & this->mask) || (elevationData[index] == 0))
      {
        // forbidden to drive here
        this->overridesMap(x, y) = 1;
        this->insertNodeInClosedSet(Node(x,y));
      }
      else
      {
        this->overridesMap(x, y) = 0;
      }
    }
  }

  // fill the base value in order to avoid a separate call.
  this->base = std::ceil(
      std::log2(static_cast<float>(this->image_XSize * this->image_YSize))) + 1;

}  //void AStar::buildCostMap(const std::vector<uint8_t>& elevationData, const std::vector<uint8_t>& overridesData)

void AStar::getMapDimensions(unsigned& x, unsigned& y) {
  x = this->image_XSize;
  y = this->image_YSize;
}

float AStar::getTerrainAt(Node n) {
  return (this->terrainMap(n.get_X(), n.get_Y()));
}

uint8_t AStar::getOverrideAt(Node n) {
  return (this->overridesMap(n.get_X(), n.get_Y()));
}

void AStar::insertNodeInOpenSet(const float cost, const Node n) {
  openList.insert(std::make_pair(cost, n));
}

bool AStar::isNodeInOpenSet(const Node n) {
  for (const std::pair<float, Node>& it : openList) {
    if (it.second == n) {
      return (true);
    }
  }

  return (false);

}

bool AStar::isOpenSetEmpty() {
  return (openList.empty());
}

Node AStar::getCheapestNodeInOpenSet() {

  // index of the node with the lowest cost
  std::map<float, Node>::iterator it = openList.begin();

  // get the values associated to this lowest-code node
  Node n = it->second;

  // remove this lowest cost node from the collection
  openList.erase(it);

  // return the result
  return (n);
}

void AStar::insertNodeInClosedSet(const Node n) {
  this->closedList(n.get_X(), n.get_Y()) = 1;
}

bool AStar::isNodeInClosedSet(const Node n) {
  return (this->closedList(n.get_X(), n.get_Y()));
}

void AStar::recordCostAt(Node n, float cost) {
  this->gMap(n.get_X(), n.get_Y()) = cost;
}

float AStar::getCostAt(Node n) {
  return (this->gMap(n.get_X(), n.get_Y()));
}

float AStar::getCostHeuristic(const Node a, const Node b) {
  float cost = this->heuristic(a, b);
  return (cost);
}

std::vector<Node> AStar::getExplorableNeighborsOf(Node n) {
  // Uses the moves vector to compute the neighbors
  allNeighbors.clear();

  Node newNeighbor(0, 0);

  for (std::pair<int, int> thisMove : moves) {
    if ((n.get_X() + thisMove.first >= 0)
        && (n.get_X() + thisMove.first < this->image_XSize)
        && (n.get_Y() + thisMove.second >= 0)
        && (n.get_Y() + thisMove.second < this->image_YSize)) {

      // neighbor is within the image. Check whether the neighbor is blocked
      newNeighbor = {n.get_X() + thisMove.first, n.get_Y() + thisMove.second};

      // this neighbor is drivable
      if(this->getOverrideAt(newNeighbor) == 0)
      {
        // drivable neighbor. Has this already been explored?
        if(! this->isNodeInClosedSet(newNeighbor))
        {
          // drivable and unexplored neighbor
          allNeighbors.push_back(newNeighbor);
        }

      }
    }
  }

  return (allNeighbors);

}  //std::vector<Node> AStar::getExplorableNeighborsOf(Node n)

void AStar::recordPath(Node target, Node source) {
  // this is a quick hack. A good solution would require computation of two separate hashes, and then a good way of combining these.
  std::string key = std::to_string(target.get_X()) + ","
      + std::to_string(target.get_Y());
  this->path.insert(std::make_pair(key, source));
}

std::vector<Node> AStar::finalPath(Node start, Node end) {
  std::vector<Node> finalPath;
  Node previous = end;
  Node now(0, 0);
  finalPath.push_back(end);

  // get the starting key
  std::string key = std::to_string(end.get_X()) + ","
      + std::to_string(end.get_Y());

  std::unordered_map<std::string, Node>::iterator it = this->path.find(key);
  while (it != this->path.end()) {
    finalPath.push_back(it->second);

    // did we already reach the starting point?
    if (it->second == start) {
      break;
    }

    // check whether the new coordinate differs from the one by only one neighbor
    now = it->second;

    // abort if the path is not continuous. Sanity check.
    assert(
        ((std::abs(
            static_cast<float>(now.get_X())
                - static_cast<float>(previous.get_X())) <= 1)
            && (std::abs(
                static_cast<float>(now.get_Y())
                    - static_cast<float>(previous.get_Y())) <= 1))
            && " Computed path has successive coordinates differing by more than 1 neighbor");

    previous = now;

    // recompute the key
    key = std::to_string((it->second).get_X()) + ","
        + std::to_string((it->second).get_Y());

    // repeat
    it = this->path.find(key);
  }

  // return the path
  return (finalPath);

}

void AStar::setMask(const uint8_t mask) {
  this->mask = mask;
}

void AStar::computeDistanceAndTimeMetrics(const std::vector<Node>& finalPath,
                                           float& totalDistanceTravelled,
                                           float& totalTimeOfTravel) {
  float sinOfElevation = 0.0f;
  totalDistanceTravelled = 0.0f;
  totalTimeOfTravel = 0.0f;
  float changeinElevation = 0.0f;
  float newdDistanceTravelled = 0.0f;
  // initialize
  Node currentNode = finalPath[0];
  std::vector<Node>::const_iterator it;

  for (it = finalPath.begin() + 1; it != finalPath.end(); ++it) {
    newdDistanceTravelled = this->getCostHeuristic(*it, currentNode);
    totalDistanceTravelled += newdDistanceTravelled;
    changeinElevation = this->getTerrainAt(*it)
        - this->getTerrainAt(currentNode);
    sinOfElevation = std::sin(
        std::atan2(changeinElevation, newdDistanceTravelled));
    totalTimeOfTravel += newdDistanceTravelled / (1 - sinOfElevation);

    currentNode = *it;

  }
}

}
/* namespace Algo */
