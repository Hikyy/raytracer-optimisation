#pragma once
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"
#include "PhongMaterial.hpp"

class CheckerMaterial : public PhongMaterial
{
private:
public:
  CheckerMaterial();
  ~CheckerMaterial();
  virtual Color getAmbient(Intersection *intersection) override;
};
