#pragma once
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"

class Scene;
class Intersection;

class Material
{
private:
public:
  float cReflection = 0;

  Material();
  ~Material();
  virtual Color render(Ray &r, Ray &camera, Intersection *intersection, Scene *scene);
};
