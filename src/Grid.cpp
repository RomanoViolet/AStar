/*
 * costGrid.cpp
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */
#include "Grid.hpp"

#include <cassert>
#include <limits>

//https://github.com/hjweide/a-star/blob/master/astar.cpp
/*
 inline Map::CostGrid(const unsigned image_XSize, const unsigned image_YSize)
 {


 }
 */

Grid::~Grid()
{
  // TODO Auto-generated destructor stub
}

void
Grid::clear()
{
  std::fill(image.begin(), image.end(), 0.0f);
}

float
Grid::operator()(int x, int y) const
{
  assert((x >= 0 && x < this->xDimension) && "Index to matrix out of bounds.");
  assert((y >= 0 && y < this->yDimension) && "Index to matrix out of bounds.");

  return (image.at(x * this->xDimension + y));
}

float&
Grid::operator()(const int x, const int y)
{
  assert((x >= 0 && x < this->xDimension) && "Index to matrix out of bounds.");
  assert((y >= 0 && y < this->yDimension) && "Index to matrix out of bounds.");

  return (image.at(x * this->xDimension + y));
}

void Grid::makeAllInfinity()
{
  std::fill(image.begin(), image.end(), std::numeric_limits<float>::infinity());
}

