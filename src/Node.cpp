/*
 * Node.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#include "Node.hpp"
#include <algorithm>
Node::Node(const unsigned x, const unsigned y)
{
  this->x = x;
  this->y = y;

}

// Creates a new object by copying the properties of an already created object of type Node
Node::Node(const Node& source)
{
  this->x = source.x;
  this->y = source.y;
}

// the assignment operator first copies the source into a temporary, and then swaps the temporary with a new object
Node&
Node::operator=(const Node& source)
{
  // invoke the oopy constructor
  Node tmp(source);

  // Swap. Use standard swap since this is not a deep copy and is not expected to throw
  std::swap(this->x, tmp.x);
  std::swap(this->y, tmp.y);

  return *this;
}

// comparison operator: return a true if two nodes are equal: That is (x,y) = (x', y')
bool
Node::operator==(const Node& other) const
{
  return ((this->x == other.x) && (this->y == other.y));
}

unsigned Node::get_X() const
{
  return(this->x);
}

unsigned Node::get_Y() const
{
  return (this->y);
}

Node::~Node()
{
  // TODO Auto-generated destructor stub
}

