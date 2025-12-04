#include <iostream>
#include "Material.hpp"
#include "Intersection.hpp"
#include "Scene.hpp"

Material::Material() : cReflection(0)
{
}

Material::~Material()
{
}

Color Material::render(Ray &r, Ray &camera, Intersection *intersection, Scene *scene)
{
  Color black;
  return black;
}