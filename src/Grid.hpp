/*
 * Map.h
 *
 *  Created on: Mar 14, 2018
 *      Author: dumbledore
 */

#ifndef Grid_HPP_
#define Grid_HPP_

#include <vector>
#include <stdexcept>

//http://www.cs.technion.ac.il/users/yechiel/c++-faq/matrix-subscript-op.html
//http://www.learncpp.com/cpp-tutorial/99-overloading-the-parenthesis-operator/
//http://en.cppreference.com/w/cpp/numeric/valarray
class Grid
{
public:
  /*Map(const unsigned image_XSize, const unsigned image_YSize): image (std::valarray<unsigned> (image_XSize, 0))
   {

   }*/

  Grid(const unsigned image_XSize, const unsigned image_YSize) :
      xDimension(image_XSize), yDimension(image_YSize), image(image_XSize * image_YSize, 0.0f)
  {
    /*if ((image_XSize == 0) || (image_YSize == 0))
    {
      throw std::invalid_argument("Image Dimensions must be positive");
    }

    this->clear();*/
  }

  virtual
  ~Grid();

  // clear the image
  void clear();

  // set all values to maximum
  void makeAllInfinity();

  // access operator: read
  float operator()(int x, int y) const;

  // access operator: write
  float& operator()(int x, int y);

  // assignment operator


private:
  int xDimension;
  int yDimension;
  std::vector<float> image;
};

#endif /* Grid_HPP_ */
