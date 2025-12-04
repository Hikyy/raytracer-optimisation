#pragma once
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"
#include "Material.hpp"

/**
 * Example of forward declaration of a class:
 * - used when we have a circular dependency between files
 */
class Scene;
class Intersection;

class PhongMaterial : public Material
{
private:
public:
  Color Ambient;
  Color Diffuse = Color(1, 1, 1);
  Color Specular = Color(1, 1, 1);
  float Shininess = 40;

  PhongMaterial();
  ~PhongMaterial();
  virtual Color render(Ray &r, Ray &camera, Intersection *intersection, Scene *scene) override;
  virtual Color getAmbient(Intersection *intersection);
};
