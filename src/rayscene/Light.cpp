#include <iostream>
#include "Light.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"

Light::Light(Vector3 c) : center(c)
{
}

Light::~Light()
{
}

Vector3 Light::GetPosition()
{
  return center;
}
