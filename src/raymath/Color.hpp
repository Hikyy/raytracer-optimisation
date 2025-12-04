#pragma once

#include <iostream>

class Color
{
public:
  Color();
  Color(float r, float g, float b);
  ~Color();

  float r = 0;
  float b = 0;
  float g = 0;

  Color operator+(Color const &col);
  Color &operator=(Color const &col);
  Color operator*(float const &f);
  Color operator*(Color const &col);
  Color operator/(float const &f);
  friend std::ostream &operator<<(std::ostream &_stream, Color const &col);
};
