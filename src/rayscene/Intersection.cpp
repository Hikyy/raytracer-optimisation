#include "Intersection.hpp"

Intersection::Intersection() : Position(Vector3()),
                               Normal(Vector3()),
                               Distance(0),
                               Mat(NULL)
{
}

Intersection::~Intersection()
{
}

Intersection &Intersection::operator=(Intersection const &inter)
{
  Position = inter.Position;
  Normal = inter.Normal;
  Distance = inter.Distance;
  Mat = inter.Mat;
  SourceRay = inter.SourceRay;
  View = inter.View;
  return *this;
}
