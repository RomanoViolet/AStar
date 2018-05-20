/*
 * Node.hpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#ifndef NODE_HPP_
#define NODE_HPP_


#include <cstdlib>

/**
 * \brief A class that provides the descriptor of a single point, currently in a 2D coordinate system
 *
 *
 */
class Node
{
public:
  // The constructor always requires an (x,y) tuple.
  Node(const unsigned x, const unsigned y);

// The copy constructor
  Node (const Node& source);

  // assignment operator
  Node& operator= (const Node& source);

  // comparison operator
  bool operator== (const Node& other) const;

  // getter methods
  unsigned get_X() const;
  unsigned get_Y() const;

  virtual
  ~Node();

private:
  unsigned x;
  unsigned y;
};



#endif /* NODE_HPP_ */
