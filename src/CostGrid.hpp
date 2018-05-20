/*
 * costGrid.h
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#ifndef COSTGRID_HPP_
#define COSTGRID_HPP_

#include <valarray>
#include <stdexcept>
#include <cassert>

//http://www.cs.technion.ac.il/users/yechiel/c++-faq/matrix-subscript-op.html
//http://www.learncpp.com/cpp-tutorial/99-overloading-the-parenthesis-operator/
//http://en.cppreference.com/w/cpp/numeric/valarray
template <typename T>
class CostGrid
{
public:
  /*CostGrid(const unsigned image_XSize, const unsigned image_YSize): image (std::valarray<unsigned> (image_XSize, 0))
   {

   }*/

  CostGrid(const unsigned image_XSize, const unsigned image_YSize) :
      xDimension(image_XSize), yDimension(image_YSize), image(image_XSize * image_YSize)
  {
    /*if ((image_XSize == 0) || (image_YSize == 0))
    {
      throw std::invalid_argument("Image Dimensions must be positive");
    }

    this->clear();*/
  }

  virtual
  ~CostGrid();

  // clear the image
  void clear();

  // set all values to maximum
  void makeAllInfinity();

  // access operator: read
  T operator()(int x, int y) const;

  // access operator: write
  T& operator()(int x, int y);

  // assignment operator


private:
  int xDimension;
  int yDimension;
  std::vector<T> image;
};

template <typename T>
CostGrid<T>::~CostGrid()
{
  // TODO Auto-generated destructor stub
}


template <typename T>
void
CostGrid<T>::clear()
{
  std::fill(image.begin(), image.end(), 0.0f);
}

template <typename T>
T
CostGrid<T>::operator()(int x, int y) const
{
/*
  assert((x >= 0 && x < this->xDimension) && "Index to matrix out of bounds.");
  assert((y >= 0 && y < this->yDimension) && "Index to matrix out of bounds.");

*/
  return (image.at(y * this->xDimension + x));
}

template <typename T>
T&
CostGrid<T>::operator()(const int x, const int y)
{
/*  assert((x >= 0 && x < this->xDimension) && "Index to matrix out of bounds.");
  assert((y >= 0 && y < this->yDimension) && "Index to matrix out of bounds.");*/

  //T res = image.at(y * this->xDimension + x);
  //return (res);
  return(image.at(y * this->xDimension + x));

}

template <typename T>
void CostGrid<T>::makeAllInfinity()
{
  if(std::numeric_limits<T>::has_infinity)
  {
    std::fill(image.begin(), image.end(), std::numeric_limits<T>::infinity());
  }
  else
  {
    std::fill(image.begin(), image.end(), std::numeric_limits<T>::max());
  }
}

#endif /* COSTGRID_HPP_ */
