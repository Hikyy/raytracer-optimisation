#pragma once
#include <string>
#include <vector>

/**
 * Configuration des scènes pour les tests
 * Définit toutes les scènes disponibles et leurs références
 */

struct SceneConfig {
    std::string name;              // Nom du test (ex: "two_spheres")
    std::string scene_path;        // Chemin vers le fichier JSON
    std::string reference_name;    // Nom de la référence (ex: "two-spheres-on-plane")
    int iterations;                // Nombre d'itérations pour benchmark
    bool has_reference;            // Si false, pas de comparaison d'image
};

class SceneRegistry {
public:
    /**
     * Obtient toutes les scènes disponibles pour les tests
     */
    static std::vector<SceneConfig> getAllScenes();
    
    /**
     * Obtient une scène spécifique par nom
     */
    static SceneConfig getScene(const std::string& name);
    
    /**
     * Obtient le chemin vers la référence selon le mode
     * @param reference_name Nom de base de la référence
     * @param use_multithread True pour mode multi-thread, False pour single-thread
     */
    static std::string getReferencePathForMode(const std::string& reference_name, bool use_multithread);
};
