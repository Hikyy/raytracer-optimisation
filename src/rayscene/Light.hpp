#pragma once
#include "SceneObject.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"

class Light
{
private:
  Vector3 center;

public:
  Light(Vector3 c);
  ~Light();

  Color Diffuse = Color(0.5, 0.5, 0.5);
  Color Specular = Color(1, 1, 1);

  Vector3 GetPosition();
};
