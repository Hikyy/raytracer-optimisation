#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"

class Material;

class Intersection
{
public:
  Vector3 Position;
  Vector3 Normal;
  float Distance;
  Ray SourceRay;
  Vector3 View;
  Material *Mat;

  Intersection();
  ~Intersection();

  Intersection &operator=(Intersection const &inters);
};
