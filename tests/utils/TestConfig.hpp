#pragma once
#include <string>

/**
 * Configuration pour les tests
 * Permet de gérer différentes configurations (single-thread, multi-thread, etc.)
 */
class TestConfig {
public:
    /**
     * Obtient le suffixe de configuration actuel
     * Basé sur les defines de compilation
     * @return Suffixe (ex: "_singlethread", "_multithread")
     */
    static std::string getConfigSuffix();
    
    /**
     * Obtient le nom de configuration lisible
     * @return Nom complet (ex: "Single-Thread", "Multi-Thread")
     */
    static std::string getConfigName();
    
    /**
     * Construit le chemin vers une image de référence en fonction de la config
     * @param test_name Nom du test
     * @return Chemin complet (ex: "../../tests/references/simple_scene_multithread.png")
     */
    static std::string getReferencePathForConfig(const std::string& test_name);
    
    /**
     * Vérifie si une référence existe pour la configuration actuelle
     * @param test_name Nom du test
     * @return true si la référence existe
     */
    static bool hasReferenceForConfig(const std::string& test_name);
};
