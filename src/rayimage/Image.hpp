#pragma once

#include <iostream>
#include "../raymath/Color.hpp"
#include <vector>

class Image
{
private:
  
  std::vector<Color> buffer;
public:
  Image(unsigned int w, unsigned int h);
  Image(unsigned int w, unsigned int h, Color c);
  ~ Image();
  unsigned int width = 0;
  unsigned int height = 0;


  void setPixel(unsigned int x, unsigned int y, Color color);
  Color getPixel(unsigned int x, unsigned int y);

  void writeFile(std::string& filename);
};
