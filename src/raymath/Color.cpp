#include <iostream>
#include "Color.hpp"

Color::Color() : r(0), b(0), g(0)
{
}

Color::Color(float iR, float iG, float iB) : r(iR), g(iG), b(iB)
{
}

Color::~Color()
{
}

/**
 * Implementation of the + operator :
 * Adding two colors is done by just adding the different components together :
 * (r1, g1, b1) + (r2, g2, b2) = (r1 + r2, g1 + g2, b1 + b2)
 */
Color Color::operator+(Color const &col)
{
  Color c;
  c.r = std::max(std::min(r + col.r, 1.0f), 0.0f);
  c.g = std::max(std::min(g + col.g, 1.0f), 0.0f);
  c.b = std::max(std::min(b + col.b, 1.0f), 0.0f);
  return c;
}

Color &Color::operator=(Color const &col)
{
  r = col.r;
  g = col.g;
  b = col.b;
  return *this;
}

Color Color::operator*(float const &f)
{
  Color c;
  c.r = std::max(std::min(r * f, 1.0f), 0.0f);
  c.g = std::max(std::min(g * f, 1.0f), 0.0f);
  c.b = std::max(std::min(b * f, 1.0f), 0.0f);
  return c;
}

Color Color::operator*(Color const &col)
{
  Color c;
  c.r = std::max(std::min(r * col.r, 1.0f), 0.0f);
  c.g = std::max(std::min(g * col.g, 1.0f), 0.0f);
  c.b = std::max(std::min(b * col.b, 1.0f), 0.0f);
  return c;
}

Color Color::operator/(float const &f)
{
  Color c;
  c.r = std::max(std::min(r / f, 1.0f), 0.0f);
  c.g = std::max(std::min(g / f, 1.0f), 0.0f);
  c.b = std::max(std::min(b / f, 1.0f), 0.0f);
  return c;
}

/**
 * Here we implement the << operator :
 * We take each component and append it to he stream, giving it a nice form on the console
 */
std::ostream &operator<<(std::ostream &_stream, Color const &col)
{
  return _stream << "(" << col.r << "," << col.g << "," << col.b << ")";
}
