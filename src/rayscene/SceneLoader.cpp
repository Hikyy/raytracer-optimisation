#include <iostream>
#include <fstream>
#include <filesystem>
#include "../json/json.hpp"
#include "SceneLoader.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Triangle.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Light.hpp"
#include "PhongMaterial.hpp"
#include "CheckerMaterial.hpp"

using json = nlohmann::json;

Vector3 parseVector3(json data)
{
    double x = data["x"];
    double y = data["y"];
    double z = data["z"];
    return Vector3(x, y, z);
}

Color parseColor(json data)
{
    double r = data["r"];
    double g = data["g"];
    double b = data["b"];
    return Color(r, g, b);
}

void parsePhongMaterialProperties(json data, PhongMaterial *mat)
{
    if (data.contains("ambient"))
    {
        mat->Ambient = parseColor(data["ambient"]);
    }
    if (data.contains("diffuse"))
    {
        mat->Diffuse = parseColor(data["diffuse"]);
    }
    if (data.contains("specular"))
    {
        mat->Specular = parseColor(data["specular"]);
    }
    if (data.contains("shininess"))
    {
        mat->Shininess = data["shininess"];
    }
    if (data.contains("reflectivity"))
    {
        mat->cReflection = data["reflectivity"];
    }
}

Material *parsePhongMaterial(json data)
{
    PhongMaterial *mat = new PhongMaterial();
    parsePhongMaterialProperties(data, mat);
    return mat;
}

Material *parseCheckerboardMaterial(json data)
{
    CheckerMaterial *mat = new CheckerMaterial();
    parsePhongMaterialProperties(data, mat);
    return mat;
}

Material *parseMaterial(json data)
{
    std::string type = data["type"];
    if (type == "phong")
    {
        return parsePhongMaterial(data);
    }
    else if (type == "checkerboard")
    {
        return parseCheckerboardMaterial(data);
    }
    return nullptr;
}

Sphere *parseSphere(json data)
{
    double radius = data["radius"];

    Sphere *s = new Sphere(radius);
    if (data.contains("position"))
    {
        Vector3 pos = parseVector3(data["position"]);
        s->transform.setPosition(pos);
    }
    if (data.contains("rotation"))
    {
        Vector3 rot = parseVector3(data["rotation"]);
        s->transform.setRotation(rot);
    }
    if (data.contains("material"))
    {
        Material *mat = parseMaterial(data["material"]);
        if (mat != nullptr)
        {
            s->material = mat;
        }
    }

    return s;
}

Plane *parsePlane(json data)
{
    Vector3 pos;
    Vector3 norm(0, 1, 0);

    if (data.contains("position"))
    {
        pos = parseVector3(data["position"]);
    }
    if (data.contains("normal"))
    {
        norm = parseVector3(data["normal"]);
    }

    Plane *plane = new Plane(pos, norm);

    if (data.contains("material"))
    {
        Material *mat = parseMaterial(data["material"]);
        if (mat != nullptr)
        {
            plane->material = mat;
        }
    }

    return plane;
}

Triangle *parseTriangle(json data)
{
    Vector3 pos;
    Vector3 rot;
    Vector3 A;
    Vector3 B;
    Vector3 C;

    if (data.contains("position"))
    {
        pos = parseVector3(data["position"]);
    }
    if (data.contains("rotation"))
    {
        rot = parseVector3(data["rotation"]);
    }

    if (data.contains("vertices"))
    {
        auto &verts = data["vertices"];
        if (!verts.is_array())
        {
            std::cerr << "vertices entry for a an object of type triangle must be an array" << std::endl;
            exit(1);
        }
        if (verts.size() != 3)
        {
            std::cerr << "vertices array for a an object of type triangle must have 3 vectors" << std::endl;
            exit(1);
        }

        A = parseVector3(verts.at(0));
        B = parseVector3(verts.at(1));
        C = parseVector3(verts.at(2));
    }

    Triangle *triangle = new Triangle(A, B, C);
    triangle->transform.setPosition(pos);
    triangle->transform.setRotation(rot);

    if (data.contains("material"))
    {
        Material *mat = parseMaterial(data["material"]);
        if (mat != nullptr)
        {
            triangle->material = mat;
        }
    }

    return triangle;
}

Mesh *parseMesh(json data, std::filesystem::path &sceneParentPath)
{

    Mesh *mesh = new Mesh();
    Vector3 pos;
    Vector3 rot;

    if (data.contains("position"))
    {
        pos = parseVector3(data["position"]);
    }
    if (data.contains("rotation"))
    {
        rot = parseVector3(data["rotation"]);
    }

    mesh->transform.setPosition(pos);
    mesh->transform.setRotation(rot);

    if (data.contains("obj"))
    {
        std::string relPath = data["obj"];
        std::filesystem::path fullPath = sceneParentPath / relPath;

        std::ifstream f(fullPath);
        if (!f.good())
        {
            std::cerr << "obj file not found at path: " << fullPath << std::endl;
            exit(1);
        }

        mesh->loadFromObj(fullPath);
    }

    if (data.contains("material"))
    {
        Material *mat = parseMaterial(data["material"]);
        if (mat != nullptr)
        {
            mesh->material = mat;
        }
    }

    return mesh;
}

void parseOjects(json data, Scene *scene, std::filesystem::path &sceneParentPath)
{
    if (!data.contains("objects"))
    {
        return;
    }

    for (auto &elem : data["objects"])
    {
        std::string type = elem["type"];
        if (type == "sphere")
        {
            Sphere *s = parseSphere(elem);
            scene->add(s);
        }
        else if (type == "plane")
        {
            Plane *p = parsePlane(elem);
            scene->add(p);
        }
        else if (type == "triangle")
        {
            Triangle *t = parseTriangle(elem);
            scene->add(t);
        }
        else if (type == "mesh")
        {
            Mesh *m = parseMesh(elem, sceneParentPath);
            scene->add(m);
        }
    }
}

Light *parsePointLight(json data)
{
    Vector3 pos;
    if (data.contains("position"))
    {
        pos = parseVector3(data["position"]);
    }

    Light *light = new Light(pos);
    if (data.contains("diffuse"))
    {
        light->Diffuse = parseColor(data["diffuse"]);
    }
    if (data.contains("specular"))
    {
        light->Specular = parseColor(data["specular"]);
    }

    return light;
}

void parseLights(json data, Scene *scene)
{
    if (!data.contains("lights"))
    {
        return;
    }

    for (auto &elem : data["lights"])
    {
        std::string type = elem["type"];
        if (type == "point")
        {
            Light *l = parsePointLight(elem);
            scene->addLight(l);
        }
    }
}

Image *parseImage(json data, Image *image)
{
    unsigned int width = 800;
    unsigned int height = 600;
    if (data.contains("image"))
    {
        json imgJson = data["image"];

        if (imgJson.contains("width"))
        {
            width = imgJson["width"];
        }

        if (imgJson.contains("height"))
        {
            height = imgJson["height"];
        }
    }

    return new Image(width, height);
}

std::tuple<Scene *, Camera *, Image *> SceneLoader::Load(std::string path)
{
    std::ifstream f(path);

    if (!f.good())
    {
        std::cerr << "Scene file not found at path: " << path << std::endl;
        exit(1);
    }

    // Get the parent directory of the scene file (for loading relative mesh files)
    std::filesystem::path fPath = path;
    std::filesystem::path parent_p = fPath.parent_path();

    json data = json::parse(f);

    Scene *scene = new Scene();
    Camera *camera = new Camera();

    parseLights(data, scene);
    parseOjects(data, scene, parent_p);

    if (data.contains("ambient"))
    {
        scene->globalAmbient = parseColor(data["ambient"]);
    }

    if (data.contains("reflections"))
    {
        camera->Reflections = data["reflections"];
    }

    Image *image = parseImage(data, image);

    return {scene, camera, image};
}