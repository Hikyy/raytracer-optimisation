#pragma once

#include <vector>
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"
#include "Light.hpp"
#include "SceneObject.hpp"
#ifdef USE_BSPTREE
#include "BSPTree.hpp"
#endif

class Scene
{
private:
  std::vector<SceneObject *> objects;
  std::vector<Light *> lights;
#ifdef USE_BSPTREE
  BSPTree bspTree;  // Arbre BSP pour optimisation des intersections
#endif

public:
  Scene();
  ~Scene();

  Color globalAmbient;

  void add(SceneObject *object);
  void addLight(Light *light);
  std::vector<Light *> getLights();

  void prepare();
  Color raycast(Ray &r, Ray &camera, int castCount, int maxCastCount);

  bool closestIntersection(Ray &r, Intersection &closest, CullingType culling);
};
