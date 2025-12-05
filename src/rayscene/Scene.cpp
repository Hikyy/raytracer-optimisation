#include <iostream>
#include "Scene.hpp"
#include "Intersection.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
  // OPTIMISÉ : Stocker la taille pour éviter appels répétés à size()
  const int objectCount = objects.size();
  for (int i = 0; i < objectCount; ++i)
  {
    delete objects[i];
  }

  const int lightCount = lights.size();
  for (int i = 0; i < lightCount; ++i)
  {
    delete lights[i];
  }
}

void Scene::add(SceneObject *object)
{
  objects.push_back(object);
}

void Scene::addLight(Light *light)
{
  lights.push_back(light);
}

void Scene::prepare()
{
  // OPTIMISÉ : Éviter appel répété à size() dans la condition de boucle
  const int objectCount = objects.size();
  for (int i = 0; i < objectCount; ++i)
  {
    objects[i]->applyTransform();
#ifdef USE_AABB
    objects[i]->calculateBoundingBox();
#endif
  }
  
#ifdef USE_BSPTREE
  // Construction de l'arbre BSP pour optimiser les recherches d'intersection
  // Complexité réduite de O(n) à O(log n) pour chaque rayon
  bspTree.build(objects);
#endif
}

std::vector<Light *> Scene::getLights()
{
  return lights;
}

/*
 * OPTIMISATION : Trouver l'intersection la plus proche avec lengthSquared()
 * 
 * CODE AVANT :
 *   double closestDistance = -1;
 *   for (...) {
 *     intersection.Distance = (intersection.Position - r.GetPosition()).length();  // sqrt() !
 *     if (closestDistance < 0 || intersection.Distance < closestDistance) {
 *       closestDistance = intersection.Distance;
 *       ...
 *     }
 *   }
 * 
 * CODE APRÈS :
 *   double closestDistanceSquared = -1;  // Stocker au carré
 *   for (...) {
 *     intersection.Distance = (...).lengthSquared();  // PAS de sqrt !
 *     if (...|| intersection.Distance < closestDistanceSquared) {  // Comparer valeurs au carré
 *       ...
 *     }
 *   }
 * 
 * Amélioration : Évite sqrt() pour chaque objet de la scène lors de la recherche de l'intersection la plus proche
 * Note : Pour comparer a < b, on peut comparer a² < b² (équivalent mathématiquement pour valeurs positives)
 */
bool Scene::closestIntersection(Ray &r, Intersection &closest, CullingType culling)
{
  Intersection intersection;

  double closestDistanceSquared = -1;  // OPTIMISÉ : Stocker distance au carré
  Intersection closestInter;

#ifdef USE_BSPTREE
  // OPTIMISATION BSP TREE : Utiliser l'arbre pour réduire le nombre d'objets à tester
  // Au lieu de tester tous les objets O(n), on ne teste que ceux dans les AABB intersectés O(log n)
  std::vector<SceneObject*> candidates;
  if (!bspTree.intersects(r, candidates)) {
    closest = closestInter;
    return false;
  }
  
  const int candidateCount = candidates.size();
  for (int i = 0; i < candidateCount; ++i)
  {
#ifdef USE_AABB
    // Double vérification avec AABB individuel (optionnel mais peut aider)
    if (!candidates[i]->boundingBox.intersects(r))
    {
      continue;
    }
#endif
    if (candidates[i]->intersects(r, intersection, culling))
    {
      intersection.Distance = (intersection.Position - r.GetPosition()).lengthSquared();
      if (closestDistanceSquared < 0 || intersection.Distance < closestDistanceSquared)
      {
        closestDistanceSquared = intersection.Distance;
        closestInter = intersection;
      }
    }
  }
#else
  // Version sans BSP Tree : tester tous les objets
  const int objectCount = objects.size();
  for (int i = 0; i < objectCount; ++i)
  {
#ifdef USE_AABB
    // OPTIMISATION AABB : Vérifier d'abord si le rayon intersecte la bounding box
    if (!objects[i]->boundingBox.intersects(r))
    {
      continue;
    }
#endif
    if (objects[i]->intersects(r, intersection, culling))
    {
      // OPTIMISÉ : Utiliser lengthSquared() au lieu de length()
      intersection.Distance = (intersection.Position - r.GetPosition()).lengthSquared();
      if (closestDistanceSquared < 0 || intersection.Distance < closestDistanceSquared)
      {
        closestDistanceSquared = intersection.Distance;
        closestInter = intersection;
      }
    }
  }
#endif

  closest = closestInter;
  return (closestDistanceSquared > -1);
}

Color Scene::raycast(Ray &r, Ray &camera, int castCount, int maxCastCount)
{

  Color pixel;

  Intersection intersection;

  if (closestIntersection(r, intersection, CULLING_FRONT))
  {
    // Add the view-ray for convenience (the direction is normalised in the constructor)
    intersection.View = (camera.GetPosition() - intersection.Position).normalize();

    if (intersection.Mat != NULL)
    {
      pixel = pixel + (intersection.Mat)->render(r, camera, &intersection, this);

      // Reflect
      if (castCount < maxCastCount & intersection.Mat->cReflection > 0)
      {
        Vector3 reflectDir = r.GetDirection().reflect(intersection.Normal);
        Vector3 origin = intersection.Position + (reflectDir * COMPARE_ERROR_CONSTANT);
        Ray reflectRay(origin, reflectDir);

        pixel = pixel + raycast(reflectRay, camera, castCount + 1, maxCastCount) * intersection.Mat->cReflection;
      }
    }
  }

  return pixel;
}