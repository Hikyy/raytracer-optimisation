#pragma once
#include <vector>
#include "SceneObject.hpp"
#include "../raymath/Transform.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"
#include "./Triangle.hpp"
#ifdef USE_BSPTREE
#include "BSPTree.hpp"
#endif

class Mesh : public SceneObject
{
private:
  std::vector<Triangle *> triangles;
#ifdef USE_BSPTREE
  BSPTree triangleBSP;  // BSP Tree pour les triangles du mesh
#endif

public:
  Mesh();
  ~Mesh();

  void loadFromObj(std::string path);

  virtual void applyTransform() override;
  virtual void calculateBoundingBox() override;
  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;
};
