#include <iostream>
#include <cmath>
#include <algorithm>
#include "Triangle.hpp"
#include "../raymath/Vector3.hpp"

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c) : SceneObject(), A(a), B(b), C(c)
{
}

Triangle::~Triangle()
{
}

void Triangle::applyTransform()
{
  tA = this->transform.apply(A);
  tB = this->transform.apply(B);
  tC = this->transform.apply(C);
}

void Triangle::calculateBoundingBox()
{
  // Calculer les min et max pour chaque axe parmi les 3 points transform\u00e9s
  Vector3 min(
    std::min({tA.x, tB.x, tC.x}),
    std::min({tA.y, tB.y, tC.y}),
    std::min({tA.z, tB.z, tC.z})
  );

  Vector3 max(
    std::max({tA.x, tB.x, tC.x}),
    std::max({tA.y, tB.y, tC.y}),
    std::max({tA.z, tB.z, tC.z})
  );

  boundingBox = AABB(min, max);
}

bool Triangle::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  Vector3 BA = tB - tA;
  Vector3 CA = tC - tA;
  Vector3 normal = BA.cross(CA).normalize();

  // Ray plane intersection
  float denom = r.GetDirection().dot(normal);

  //
  // If denom == 0 - it is parallel to the plane
  // If denom > 0, it means plane is behind the ray
  if (culling == CULLING_FRONT && denom > -0.000001)
  {
    return false;
  }
  if (culling == CULLING_BACK && denom < 0.000001)
  {
    return false;
  }

  float numer = (tA - r.GetPosition()).dot(normal);
  float t = numer / denom;

  // Behind the ray
  if (t <= 0)
  {
    return false;
  }

  // Point on plane
  Vector3 Q = r.GetPosition() + (r.GetDirection() * t);

  // Point contained in triangle
  Vector3 QA = Q - tA;
  Vector3 BAxQA = BA.cross(QA);
  if (BAxQA.dot(normal) < 0)
  {
    return false;
  }

  Vector3 CB = tC - tB;
  Vector3 QB = Q - tB;
  Vector3 CBxQB = CB.cross(QB);
  if (CBxQB.dot(normal) < 0)
  {
    return false;
  }

  Vector3 AC = tA - tC;
  Vector3 QC = Q - tC;
  Vector3 ACxQC = AC.cross(QC);
  if (ACxQC.dot(normal) < 0)
  {
    return false;
  }

  intersection.Position = Q;
  intersection.Mat = this->material;
  intersection.Normal = normal;

  return true;
}