#pragma once
#include <vector>
#include "../raymath/AABB.hpp"
#include "../raymath/Ray.hpp"
#include "SceneObject.hpp"

/**
 * BSP Tree (Binary Space Partition Tree)
 * 
 * Optimisation de la complexité de recherche d'intersection rayon-objet.
 * Au lieu de tester tous les objets (O(n)), on divise l'espace récursivement
 * pour atteindre une complexité O(log n).
 * 
 * Principe:
 * - L'espace est divisé en deux récursivement
 * - Chaque nœud intermédiaire contient un AABB englobant ses enfants
 * - Seuls les nœuds feuilles contiennent des objets
 * - Lors du lancer de rayon, on ignore les branches dont l'AABB n'est pas intersecté
 */

class BSPNode {
public:
    AABB boundingBox;           // AABB englobant tout ce nœud
    BSPNode* left = nullptr;    // Enfant gauche
    BSPNode* right = nullptr;   // Enfant droit
    std::vector<SceneObject*> objects;  // Objets (seulement pour les feuilles)
    bool isLeaf = false;
    
    BSPNode();
    ~BSPNode();
};

class BSPTree {
public:
    BSPTree();
    ~BSPTree();
    
    /**
     * Construit l'arbre BSP à partir d'une liste d'objets
     * @param objects Liste des objets de la scène
     * @param maxDepth Profondeur maximale de l'arbre
     * @param minObjects Nombre minimum d'objets par feuille
     */
    void build(std::vector<SceneObject*>& objects, int maxDepth = 10, int minObjects = 2);
    
    /**
     * Trouve les objets potentiellement intersectés par un rayon
     * @param ray Le rayon à tester
     * @param candidates Liste où seront ajoutés les objets candidats
     * @return true si des candidats ont été trouvés
     */
    bool intersects(Ray& ray, std::vector<SceneObject*>& candidates);
    
private:
    BSPNode* root = nullptr;
    
    /**
     * Construit récursivement un nœud de l'arbre
     */
    BSPNode* buildRecursive(std::vector<SceneObject*>& objects, int depth, int maxDepth, int minObjects);
    
    /**
     * Calcule l'AABB englobant tous les objets
     */
    AABB computeBoundingBox(std::vector<SceneObject*>& objects);
    
    /**
     * Trouve l'axe le plus long de l'AABB pour la division
     */
    int findSplitAxis(AABB& box);
    
    /**
     * Traverse l'arbre récursivement pour trouver les candidats
     */
    void traverseRecursive(BSPNode* node, Ray& ray, std::vector<SceneObject*>& candidates);
    
    /**
     * Libère la mémoire de l'arbre
     */
    void destroyRecursive(BSPNode* node);
};
