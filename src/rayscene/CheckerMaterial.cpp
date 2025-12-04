#include <iostream>
#include <cmath>
#include "CheckerMaterial.hpp"
#include "Intersection.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Intersection.hpp"

CheckerMaterial::CheckerMaterial()
{
}

CheckerMaterial::~CheckerMaterial()
{
}

Color CheckerMaterial::getAmbient(Intersection *intersection)
{
  float f = ((int)floorf(intersection->Position.x) % 2 == 0 && (int)floorf(intersection->Position.z) % 2 != 0) ||
                    ((int)floorf(intersection->Position.x) % 2 != 0 && (int)floorf(intersection->Position.z) % 2 == 0)
                ? 1
                : 0;

  return Ambient * f;
}
