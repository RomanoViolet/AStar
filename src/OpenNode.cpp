/*
 * openNodes.cpp
 *
 *  Created on: Mar 16, 2018
 *      Author: rad2rng
 */

#include "OpenNode.hpp"

OpenNode::~OpenNode() {
  // TODO Auto-generated destructor stub
}

float OpenNode::getCost() const
{
  return(this->cost);
}

Node OpenNode::getNode() const
{
  return(this->node);
}
