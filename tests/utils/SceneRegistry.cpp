#include "SceneRegistry.hpp"

std::vector<SceneConfig> SceneRegistry::getAllScenes() {
    return {
        {
            "two_spheres",
            "scenes/two-spheres-on-plane.json",
            "two-spheres-on-plane",
            3,    // 3 itérations
            true  // A une référence
        },
        {
            "triangles",
            "scenes/two-triangles-on-plane.json",
            "two-triangles-on-plane",
            2,
            true
        },
        {
            "iso_sphere_mesh",
            "scenes/iso-sphere-on-plane.json",
            "iso-sphere-on-plane",
            2,
            true
        },
        {
            "monkey_mesh",
            "scenes/monkey-on-plane.json",
            "monkey-on-plane",
            2,
            true
        },
        {
            "galaxy",
            "scenes/sphere-galaxy-on-plane.json",
            "sphere-galaxy-on-plane",
            1,     // 1 itération (scène lente)
            true   // A maintenant une référence!
        }
    };
}

SceneConfig SceneRegistry::getScene(const std::string& name) {
    auto scenes = getAllScenes();
    for (const auto& scene : scenes) {
        if (scene.name == name) {
            return scene;
        }
    }
    // Retourne une scène par défaut si pas trouvé
    return scenes[0];
}

std::string SceneRegistry::getReferencePathForMode(const std::string& reference_name, bool use_multithread) {
    return "tests/references/" + reference_name + ".png";
}
