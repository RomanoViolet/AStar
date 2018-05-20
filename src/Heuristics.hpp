/*
 * heuristics.hpp
 *
 *  Created on: Mar 16, 2018
 *      Author: rad2rng
 */

#ifndef HEURISTICS_HPP_
#define HEURISTICS_HPP_

#include "Node.hpp"

class Heuristics
{
 public:
  Heuristics();
  virtual ~Heuristics();

  // used both for heuristics and movement costs
  float EuclideanDistance(const Node& a, const Node& b);
};





#endif /* HEURISTICS_HPP_ */
