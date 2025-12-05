#include <iostream>
#include <chrono>
#include <string>
#include "SceneLoader.hpp"

int main(int argc, char *argv[])
{
  std::cout << std::endl;
  std::cout << "*********************************" << std::endl;
  std::cout << "*** Kevin's Awesome Raytracer ***" << std::endl;
  std::cout << "*********************************" << std::endl;
  std::cout << std::endl;

  if (argc < 2)
  {
    std::cerr << "[ERROR] Please a path your scene file (.json)" << std::endl;
    std::cout << std::endl;
    exit(0);
  }

  std::string path = argv[1];
  auto [scene, camera, image] = SceneLoader::Load(path);

  std::string outpath = "image.png";

  // Parser les arguments optionnels pour le chemin de sortie
  if (argc >= 3)
  {
    outpath = argv[2];
  }

#ifdef USE_MULTITHREADING
  std::cout << "Mode: Multi-threaded" << std::endl;
#else
  std::cout << "Mode: Single-threaded" << std::endl;
#endif

#ifdef USE_AABB
  std::cout << "AABB Optimization: ON" << std::endl;
#else
  std::cout << "AABB Optimization: OFF" << std::endl;
#endif

  std::cout << "Rendering " << image->width << "x" << image->height << " pixels..." << std::endl;

  auto begin = std::chrono::high_resolution_clock::now();
  camera->render(*image, *scene);
  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

  std::cout << "Done." << std::endl;
  std::printf("Total time: %.3f seconds.\n", elapsed.count() * 1e-9);

  std::cout << "Writing file: " << outpath << std::endl;
  image->writeFile(outpath);

  delete scene;
  delete camera;
  delete image;
}
