#pragma once

#include <tuple>
#include "Scene.hpp"
#include "Camera.hpp"
#include "../rayimage/Image.hpp"

class SceneLoader
{
public:
    static std::tuple<Scene *, Camera *, Image *> Load(std::string path);
};
