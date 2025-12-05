#include <algorithm>
#include <iostream>
#include <limits>
#include "BSPTree.hpp"

// ============================================================================
// BSPNode Implementation
// ============================================================================

BSPNode::BSPNode() : left(nullptr), right(nullptr), isLeaf(false) {}

BSPNode::~BSPNode() {
    // Les objets ne sont pas détruits ici, ils appartiennent à la Scene
}

// ============================================================================
// BSPTree Implementation
// ============================================================================

BSPTree::BSPTree() : root(nullptr) {}

BSPTree::~BSPTree() {
    if (root) {
        destroyRecursive(root);
    }
}

void BSPTree::destroyRecursive(BSPNode* node) {
    if (!node) return;
    
    destroyRecursive(node->left);
    destroyRecursive(node->right);
    delete node;
}

/**
 * Construit l'arbre BSP à partir des objets de la scène
 * 
 * ALGORITHME:
 * 1. Calculer l'AABB englobant tous les objets
 * 2. Si peu d'objets ou profondeur max: créer une feuille
 * 3. Sinon: diviser l'espace en deux selon l'axe le plus long
 * 4. Récurser sur chaque moitié
 */
void BSPTree::build(std::vector<SceneObject*>& objects, int maxDepth, int minObjects) {
    if (objects.empty()) {
        root = nullptr;
        return;
    }
    
    root = buildRecursive(objects, 0, maxDepth, minObjects);
}

BSPNode* BSPTree::buildRecursive(std::vector<SceneObject*>& objects, int depth, int maxDepth, int minObjects) {
    if (objects.empty()) {
        return nullptr;
    }
    
    BSPNode* node = new BSPNode();
    node->boundingBox = computeBoundingBox(objects);
    
    // Condition d'arrêt: peu d'objets ou profondeur max atteinte
    if (objects.size() <= static_cast<size_t>(minObjects) || depth >= maxDepth) {
        node->isLeaf = true;
        node->objects = objects;
        return node;
    }
    
    // Trouver l'axe de division (le plus long)
    int axis = findSplitAxis(node->boundingBox);
    
    // Trier les objets selon le centre de leur bounding box sur cet axe
    std::sort(objects.begin(), objects.end(), [axis](SceneObject* a, SceneObject* b) {
        // Calculer le centre de la bounding box
        Vector3 centerA = (a->boundingBox.getMin() + a->boundingBox.getMax()) * 0.5;
        Vector3 centerB = (b->boundingBox.getMin() + b->boundingBox.getMax()) * 0.5;
        
        if (axis == 0) return centerA.x < centerB.x;
        if (axis == 1) return centerA.y < centerB.y;
        return centerA.z < centerB.z;
    });
    
    // Diviser au milieu
    size_t mid = objects.size() / 2;
    
    std::vector<SceneObject*> leftObjects(objects.begin(), objects.begin() + mid);
    std::vector<SceneObject*> rightObjects(objects.begin() + mid, objects.end());
    
    // Récursion
    node->left = buildRecursive(leftObjects, depth + 1, maxDepth, minObjects);
    node->right = buildRecursive(rightObjects, depth + 1, maxDepth, minObjects);
    
    return node;
}

AABB BSPTree::computeBoundingBox(std::vector<SceneObject*>& objects) {
    if (objects.empty()) {
        return AABB();
    }
    
    // Commencer avec la bounding box du premier objet
    AABB result = objects[0]->boundingBox;
    
    // Englober tous les autres objets
    for (size_t i = 1; i < objects.size(); ++i) {
        result.subsume(objects[i]->boundingBox);
    }
    
    return result;
}

int BSPTree::findSplitAxis(AABB& box) {
    // Trouver l'axe avec la plus grande étendue
    Vector3 min = box.getMin();
    Vector3 max = box.getMax();
    
    double extentX = max.x - min.x;
    double extentY = max.y - min.y;
    double extentZ = max.z - min.z;
    
    // Retourner l'axe le plus long
    if (extentX >= extentY && extentX >= extentZ) return 0;  // X
    if (extentY >= extentX && extentY >= extentZ) return 1;  // Y
    return 2;  // Z
}

/**
 * Trouve les objets potentiellement intersectés par un rayon
 * 
 * ALGORITHME:
 * 1. Si le rayon n'intersecte pas l'AABB du nœud: retourner
 * 2. Si c'est une feuille: ajouter tous ses objets aux candidats
 * 3. Sinon: récurser sur les enfants dont l'AABB est intersecté
 */
bool BSPTree::intersects(Ray& ray, std::vector<SceneObject*>& candidates) {
    candidates.clear();
    
    if (!root) {
        return false;
    }
    
    traverseRecursive(root, ray, candidates);
    return !candidates.empty();
}

void BSPTree::traverseRecursive(BSPNode* node, Ray& ray, std::vector<SceneObject*>& candidates) {
    if (!node) return;
    
    // Test d'intersection avec l'AABB du nœud
    if (!node->boundingBox.intersects(ray)) {
        // Le rayon n'intersecte pas cette partie de l'espace
        // On peut ignorer toute cette branche! (OPTIMISATION CLÉ)
        return;
    }
    
    if (node->isLeaf) {
        // Feuille: ajouter tous les objets comme candidats
        for (SceneObject* obj : node->objects) {
            candidates.push_back(obj);
        }
    } else {
        // Nœud intermédiaire: récurser sur les deux enfants
        traverseRecursive(node->left, ray, candidates);
        traverseRecursive(node->right, ray, candidates);
    }
}
