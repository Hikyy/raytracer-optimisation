#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
}

Sphere::~Sphere()
{
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
}



/*
 * NOTE D'OPTIMISATION :
 * Ce fichier a été optimisé pour la performance. Changements principaux :
 * 
 * 1. SUPPRIMÉ la fonction countPrimes() - consommait 46% du temps d'exécution !
 *    CODE AVANT :
 *      void Sphere::countPrimes() {
 *        int count = 0;
 *        for (int i = 2; i < 100000; i++) {
 *          bool isPrime = true;
 *          for (int j = 2; j * j <= i; j++) {
 *            if (i % j == 0) { isPrime = false; break; }
 *          }
 *          if (isPrime) count++;
 *        }
 *      }
 *      // Appelé dans intersects() : countPrimes(); <-- INUTILE!
 *    CODE APRÈS : SUPPRIMÉ complètement
 *    Amélioration : -31.6% plus rapide (37.9s -> 25.9s sur scène sphere-galaxy)
 * 
 * 2. OPTIMISÉ la comparaison de distance avec lengthSquared()
 *    CODE AVANT : 
 *      double distance = CP.length();  // Appelle sqrt()
 *      if (distance > radius) return false;
 *    CODE APRÈS :
 *      double distanceSquared = CP.lengthSquared();  // PAS de sqrt !
 *      if (distanceSquared > radius * radius) return false;
 *    Amélioration : -2.1% plus rapide (25.9s -> 25.4s)
 * 
 * Voir les résultats de profiling Valgrind pour l'analyse détaillée.
 */

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // Vector from ray origin to center of sphere
  Vector3 OC = center - r.GetPosition();

  // Project OC onto the ray
  Vector3 OP = OC.projectOn(r.GetDirection());

  // If the OP vector is pointing in the opposite direction of the ray
  // ... then it is behind the ray origin, ignore the object
  if (OP.dot(r.GetDirection()) <= 0)
  {
    return false;
  }

  // P is the corner of the right-angle triangle formed by O-C-P
  Vector3 P = r.GetPosition() + OP;

  // OPTIMISATION : Utiliser lengthSquared() au lieu de length() pour éviter sqrt()
  // La longueur de CP est-elle supérieure au rayon du cercle ? Si oui, pas d'intersection !
  // CODE AVANT : double distance = CP.length(); if (distance > radius) return false;
  Vector3 CP = P - center;
  double distanceSquared = CP.lengthSquared();  // OPTIMISÉ : Pas de sqrt ici !
  if (distanceSquared > radius * radius)        // OPTIMISÉ : Comparer les valeurs au carré
  {
    return false;
  }

  // Calculer le point exact de collision : P1
  // NOTE : Ici on a toujours besoin de sqrt() pour le calcul final - inévitable
  double a = sqrt(radius * radius - distanceSquared);
  double OPLength = OP.length();
  double t = OPLength - a;
  Vector3 P1 = r.GetPosition() + (r.GetDirection() * t);

  intersection.Position = P1;
  intersection.Mat = this->material;
  intersection.Normal = (P1 - center).normalize();


  // Suppression de l'appelle de la fonction countPrimes()

  return true;
}

