/*
 * heuristics.cpp
 *
 *  Created on: Mar 16, 2018
 *      Author: rad2rng
 */
#include <cmath>
#include "Heuristics.hpp"

Heuristics::Heuristics()
{

}

Heuristics::~Heuristics()
{

}

float Heuristics::EuclideanDistance(const Node& a, const Node& b)
{
  float p1 = static_cast<float>(a.get_X()) - static_cast<float>(b.get_X());
  float p2 = std::pow(p1, 2.0);

  float p3 = static_cast<float>(a.get_Y()) - static_cast<float>(b.get_Y());
  float p4 = std::pow(p3, 2.0);

  float result = std::pow(p2 + p4, 0.5);

  return(result);
  //return (std::pow( std::pow( static_cast<float>(a.get_X()) - static_cast<float>(b.get_X()), 2.0 ) + std::pow( static_cast<float>(a.get_Y()) - static_cast<float>(b.get_Y()), 2.0 ), 0.5));
}
