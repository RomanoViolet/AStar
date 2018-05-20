/*
 * AStar.hpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

// Credits: Finding minimum on a map: https://stackoverflow.com/questions/1660195/c-how-to-find-the-biggest-key-in-a-stdmap
#ifndef ASTAR_HPP_
#define ASTAR_HPP_

#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Heuristics.hpp"
#include "CostGrid.hpp"
#include "Node.hpp"

/*
 * Core Algorithms. Contains only the A* search algorithm.
 */

namespace Algo
{

/*
 * Computes shortest path between two points on unstructured data, and accepts a heuristics to compute the "f"-score.
 * Also expects to be provided a vector containing (delta x, delta y) moves that the robot is allowed to make in this environment.
 */

class AStar
{
public:
  AStar(std::function<float(const Node &a, const Node &b)> heuristic, // the heuristic
        unsigned image_XSize, unsigned image_YSize,                   // Dimensions of the provided map on which the path has to be found
        const std::vector<std::pair<int, int>> &moves)                // the vector indicating possible moves that the robot is allowed to make
      : image_XSize(image_XSize),
        image_YSize(image_YSize),
        heuristic(heuristic),
        terrainMap(image_XSize, image_YSize),
        overridesMap(image_XSize, image_YSize),
        mask(0x0),
        moves(moves),
        closedList(image_XSize, image_YSize),
        gMap(image_XSize, image_YSize),
        path(),
        base(0.0f),
        timeDelayFactor(0.0f)
  {

    // reserve space for computing neighbors, avoiding resizing.
    allNeighbors.reserve(moves.size());

    // initialize the gMap
    gMap.makeAllInfinity();
  }

  virtual ~AStar();

  // Method to compute terrain and overrides map from the serial data.
  /*
   * @param: serialized elevation data
   * @param: serialialized overrides data
   */
  void
  buildTerrainMap(const std::vector<uint8_t> &elevationData,
                  const std::vector<uint8_t> &overridesData);

  /*
   * Provides the dimensions of the map as provided during creation of AStar object.
   */
  void getMapDimensions(unsigned &x, unsigned &y);

  /*
   * Provides the elevation data at a location <Node> n. See class Node for details.
   * @param: Location where terrain information is requested at <Node> n.
   */
  float getTerrainAt(Node n);

  /*
   * Provides override information at a location <Node> n. See class Node for details.
   * @param: Location where terrain information is requested at <Node> n.
   */
  uint8_t getOverrideAt(Node n);

  /*
   * Insert a <Node> n with f-score "cost" in the internally managed open-list.
   * @param: f-score of the <Node> n to be inserted
   * @param: the <Node> n to be inserted
   */
  void insertNodeInOpenSet(const float cost, const Node n);

  /*
   * Returns true if the <Node> n is contained in the open-list.
   * @param: <Node> n
   */
  bool isNodeInOpenSet(const Node n);

  /*
   * Returs true is the open-list is empty
   */
  bool isOpenSetEmpty();

  /*
   * Returns a node of type <Node> with the lowest f-score from the nodes in open-list.
   */
  Node getCheapestNodeInOpenSet();

  /*
   * Inserts a <Node> n in the list of already explored nodes.
   * @param: The <Node> n to be inserted.
   */
  void insertNodeInClosedSet(const Node n);

  /*
   * Returns true if the <Node> n has already been explored, i.e., it is contained in the closet-list.
   * @param <Node> n to be queried.
   */
  bool isNodeInClosedSet(const Node n);

  /*
   * Update the g-score ("cost") associated with the <Node> n.
   * @param: The location given by <Node> n whose g-score needs an update
   * @param: The new assocaited g-score.
   */
  void recordCostAt(Node n, float cost);

  /*
   * Retrieve the g-score associated with <Node> n.
   * @param The location <Node> n for which the g-score is requested.
   */
  float getCostAt(Node n);

  /*
   * Compute the heuristic cost (h-score) between two locations a, and b.
   * Uses a user-supplied function.
   * @param: Location a, type <Node>
   * @param: Location b, type <Node>
   */
  float getCostHeuristic(const Node a, const Node b);

  /*
   * Returns a list of unexplored neighbors (e.g., pixels) of location <Node> n
   * which are drivable, and have not already been explored.
   * @param: The location <Node> n whose neighbors need to be searched.
   */
  std::vector<Node> getExplorableNeighborsOf(Node n);

  /*
   * Records the linkage between target and source locations. Specifically, it holds
   * the information that the lowest cost of approaching the <Node> target is from <Node> source.
   * @param: The target node reached
   * @param  The neighbor "source" from which the target can be reached with the lowest cost.
   */
  void recordPath(Node target, Node source);

  /*
   * Computed the final path as a sequence of <Nodes> backwards from end -> start locations.
   * @param: the final destination "end" to which the path needs to be elaborated
   * @param: The source location "start" from which the path to the destination must be found.
   */
  std::vector<Node> finalPath(Node start, Node end);

  /*
   * Stores the mask indicating features on the terrain e.g., rivers.
   * @param: mask as supplied by the user.
   */
  void setMask(const uint8_t mask);

  /*
   * Computes the overall times and distance required to make the journey.
   * Uses simple heuristics: The time required to travel a distance 1 with an
   * elevation of <alpha> is 1/(1 - sin(alpha)).
   * The distance travelled is the Euclidean measure between two points on the
   * map. It is calculated from the finalPath returned by the class.
   * @param: finalPath as returned by the class, see above
   * @param: [out] total distance travelled.
   * @param: [out] normalized total time required to travel the distance.
   */
  void computeDistanceAndTimeMetrics(const std::vector<Node> &finalPath,
                                     float &totalDistanceTravelled,
                                     float &totalTimeOfTravel);

private:
  // size of the image to search
  unsigned image_XSize;
  unsigned image_YSize;

  // heuristic function to be used
  std::function<float(const Node &a, const Node &b)> heuristic;

  // The cost map
  CostGrid<float> terrainMap;

  // The cost map
  CostGrid<uint8_t> overridesMap;

  // the masks used to compute areas where Rover cannot go
  uint8_t mask;

  // all possible moves the rover can make
  const std::vector<std::pair<int, int>> &moves;

  // openlist. We use a map to leverage map's property to keep things sorted.
  std::map<float, Node> openList;

  // implements closedlist
  CostGrid<uint8_t> closedList;

  // The cost map
  CostGrid<float> gMap;

  /* Vector to hold all neighbors. Do not need dynamic allocation, so we choose
   * simpler type*/
  std::vector<Node> allNeighbors;

  // stores the path. The target node is converted to string in order to use
  // built-in hashing function
  std::unordered_map<std::string, Node> path;

  // base to the key generated to be used with unordered_pair
  float base;

  // scalar to compute the effect of change in elevation as the rover drives
  float timeDelayFactor;
};

} /* namespace Algo */

#endif /* ASTAR_HPP_ */
