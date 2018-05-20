/*
 * SearchPath.hpp
 *
 *  Created on: Mar 17, 2018
 *      Author: dumbledore
 */

#ifndef SEARCHPATH_HPP_
#define SEARCHPATH_HPP_

#include <vector>
#include "Node.hpp"

/*
 * Searches path between two endpoints, given elevation and overrides data
 * Returns a vector of coordinates forming the path.
 * @param: "start" : The starting location <Node> start from which the path has to be searched.
 * @param: "end" : The destination to which the path must be computed
 * @param: "image_XSize": The width of the map, in pixels
 * @param: "image_YSize" : The height of the map, in pixels
 * @param: "elevationData" : Elevation data as provided by AID.
 * @param: "overridesData" : Overrides data as provided by AID.
 * @param: "mask": Override masks, e.g., OF_RIVER_MARSH as provided by AID.
 * @param: "moves": A vector of (deltaX, deltaY) movements the robot can make at each time step. Degree of Freedom of the robot.
 * @param: [out] "path": The path as a sequence of type <Node> backwards from Destination - ... - Source.
 * @param: [out] "totalDistanceTravelled": Total distance of the path in island-units.
 * @param: [out] "totalTimeOfTravel": Total time required to compute the distance of the path computed in 'island-ticks'
 */

// the return will be via the move semantics
void
searchPath(const Node start, const Node end, const unsigned image_XSize,
           const unsigned image_YSize,
           const std::vector<uint8_t>& elevationData,
           const std::vector<uint8_t>& overridesData,
           const uint8_t mask,
           const float weight,
           const std::vector<std::pair<int, int>>& moves, std::vector<Node>& path, float& totalDistanceTravelled, float& totalTimeOfTravel);

#endif /* SEARCHPATH_HPP_ */
