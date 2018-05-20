/*
 * openNodes.hpp
 *
 *  Created on: Mar 16, 2018
 *      Author: rad2rng
 */

#ifndef SRC_OPENNODE_HPP_
#define SRC_OPENNODE_HPP_

#include "Node.hpp"

class OpenNode {
 public:
  OpenNode(const float cost, const Node node) : cost(cost), node(node)
 {

 }
  virtual ~OpenNode();

  float getCost() const;
  Node getNode() const;

 private:
  float cost;
  Node node;

};

#endif /* SRC_OPENNODE_HPP_ */
