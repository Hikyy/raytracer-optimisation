#include <iostream>
#include "SceneObject.hpp"
#include "Intersection.hpp"

SceneObject::SceneObject() : material(NULL)
{
}

SceneObject::~SceneObject()
{
}

bool SceneObject::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  return false;
}

void SceneObject::applyTransform()
{
}

void SceneObject::calculateBoundingBox()
{
}
