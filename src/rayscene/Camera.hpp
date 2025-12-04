#pragma once

#include <iostream>
#include "../raymath/Vector3.hpp"
#include "../rayimage/Image.hpp"
#include "../rayscene/Scene.hpp"

class Camera
{
private:
  Vector3 position;

public:
  Camera();
  Camera(Vector3 pos);
  ~Camera();

  int Reflections = 0;

  Vector3 getPosition();
  void setPosition(Vector3 &pos);

  void render(Image &image, Scene &scene);

  friend std::ostream &operator<<(std::ostream &_stream, Vector3 const &vec);
};
