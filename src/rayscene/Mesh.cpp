#include <iostream>
#include <limits>
#include "Mesh.hpp"
#include "../raymath/Vector3.hpp"
#include "../objloader/OBJ_Loader.h"

Mesh::Mesh() : SceneObject()
{
}

Mesh::~Mesh()
{
    const int count = triangles.size();
    for (int i = 0; i < count; ++i)
    {
        delete triangles[i];
    }
}

void Mesh::loadFromObj(std::string path)
{

    objl::Loader *loader = new objl::Loader();
    bool loadout = loader->LoadFile(path);

    if (loadout)
    {
        for (int i = 0; i < loader->LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = loader->LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                Vector3 v1(
                    curMesh.Vertices[curMesh.Indices[j]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                Vector3 v2(
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                Vector3 v3(
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);

                Triangle *triangle = new Triangle(
                    v1,
                    v2,
                    v3);
                triangle->name = "T:" + std::to_string(j);
                triangle->ID = j;
                triangles.push_back(triangle);
            }
        }
    }

    this->applyTransform();
    delete loader;
}

void Mesh::applyTransform()
{
    const int count = triangles.size();
    for (int i = 0; i < count; ++i)
    {
        triangles[i]->material = this->material;
        triangles[i]->transform = transform;
        triangles[i]->applyTransform();
    }
}

void Mesh::calculateBoundingBox()
{
    if (triangles.empty())
    {
        boundingBox = AABB(Vector3(), Vector3());
        return;
    }

    // Calculer les bounding boxes de tous les triangles
    const int count = triangles.size();
    triangles[0]->calculateBoundingBox();
    boundingBox = triangles[0]->boundingBox;

    for (int i = 1; i < count; ++i)
    {
        triangles[i]->calculateBoundingBox();
        boundingBox.subsume(triangles[i]->boundingBox);
    }

#ifdef USE_BSPTREE
    // Construire le BSP Tree pour les triangles du mesh
    // Convertir les triangles en SceneObject* pour le BSP Tree
    std::vector<SceneObject*> triangleObjects;
    triangleObjects.reserve(count);
    for (int i = 0; i < count; ++i)
    {
        triangleObjects.push_back(triangles[i]);
    }
    triangleBSP.build(triangleObjects, 15, 4);  // Plus de profondeur pour les triangles
#endif
}

bool Mesh::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
#ifdef USE_AABB
    // OPTIMISATION CRITIQUE : Vérifier d'abord la bounding box du mesh ENTIER
    // Si le rayon ne touche pas le mesh, on évite de tester les 967 triangles !
    if (!boundingBox.intersects(r))
    {
        return false;
    }
#endif

    Intersection tInter;
    double closestDistanceSquared = -1;
    Intersection closestInter;

#ifdef USE_BSPTREE
    // OPTIMISATION: Vecteur statique pour éviter réallocations (hot path!)
    static thread_local std::vector<SceneObject*> candidates;
    candidates.clear();
    
    if (!triangleBSP.intersects(r, candidates))
    {
        return false;
    }

    const int candidateCount = candidates.size();
    for (int i = 0; i < candidateCount; ++i)
    {
#ifdef USE_AABB
        // Vérifier l'AABB du triangle d'abord
        if (!candidates[i]->boundingBox.intersects(r))
        {
            continue;
        }
#endif
        if (candidates[i]->intersects(r, tInter, culling))
        {
            tInter.Distance = (tInter.Position - r.GetPosition()).lengthSquared();
            if (closestDistanceSquared < 0 || tInter.Distance < closestDistanceSquared)
            {
                closestDistanceSquared = tInter.Distance;
                closestInter = tInter;
            }
        }
    }
#else
    // Version sans BSP Tree: tester tous les triangles
    const int count = triangles.size();
    for (int i = 0; i < count; ++i)
    {
#ifdef USE_AABB
        // Optimisation AABB: vérifier la bounding box du triangle
        if (!triangles[i]->boundingBox.intersects(r))
        {
            continue;
        }
#endif
        if (triangles[i]->intersects(r, tInter, culling))
        {
            tInter.Distance = (tInter.Position - r.GetPosition()).lengthSquared();
            if (closestDistanceSquared < 0 || tInter.Distance < closestDistanceSquared)
            {
                closestDistanceSquared = tInter.Distance;
                closestDistanceSquared = tInter.Distance;
                closestInter = tInter;
            }
        }
    }
#endif

    if (closestDistanceSquared < 0)
    {
        return false;
    }

    intersection = closestInter;
    return true;
}