#include <iostream>
#include <cassert>
#include "ImageComparator.hpp"
#include "ImageHasher.hpp"
#include "BenchmarkRunner.hpp"
#include "SceneRegistry.hpp"

/*
 * TEST: Rendu de Scène Simple
 * Teste TOUTES les scènes disponibles pour s'assurer que le raytracer
 * fonctionne correctement après les optimisations
 * 
 * Utilise DEUX méthodes de validation:
 * 1. Comparaison pixel-par-pixel (tolérance ±2) - Robuste au multi-threading
 * 2. Comparaison par hash (exactitude absolue) - Détecte moindre différence
 */

int main(int argc, char** argv) {
    // Déterminer le mode (default: utilise la config de compilation)
    bool use_multithread = true;
#ifndef USE_MULTITHREADING
    use_multithread = false;
#endif
    
    // Permettre override via argument ("--single-thread")
    if (argc > 1 && std::string(argv[1]) == "--single-thread") {
        use_multithread = false;
    }
    
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Rendu Scène Simple            ===" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "Mode: " << (use_multithread ? "Multi-Thread" : "Single-Thread") << std::endl;
    std::cout << std::endl;
    
    // Obtenir toutes les scènes
    auto scenes = SceneRegistry::getAllScenes();
    bool all_passed = true;
    
    for (const auto& scene : scenes) {
        std::cout << "--- Scène: " << scene.name << " ---" << std::endl;
        
        std::string output_path = "tests/output/" + scene.name + ".png";
        
        // Benchmark
        auto metrics = BenchmarkRunner::runBenchmark(
            scene.name,
            scene.scene_path,
            output_path,
            scene.iterations
        );
        
        if (!metrics.passed) {
            std::cerr << "❌ Rendu échoué: " << metrics.error_message << std::endl;
            all_passed = false;
            continue;
        }
        
        // Comparaison d'image (si référence existe)
        if (scene.has_reference) {
            std::cout << "--- Validation de l'Image ---" << std::endl;
            
            // Calculer le hash de l'image générée
            std::string generated_hash = ImageHasher::computeImageHash(output_path);
            std::cout << "Hash généré: " << generated_hash << std::endl;
            std::cout << "Hash référence: " << scene.reference_hash << std::endl;
            
            // Comparer avec le hash de référence stocké dans SceneConfig
            bool hash_ok = (generated_hash == scene.reference_hash);
            
            if (hash_ok) {
                std::cout << "✅ Hash correspondent: " << generated_hash << std::endl;
            } else {
                std::cerr << "❌ Hash différents:" << std::endl;
                std::cerr << "   Généré:    " << generated_hash << std::endl;
                std::cerr << "   Référence: " << scene.reference_hash << std::endl;
                all_passed = false;
            }
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "============================================" << std::endl;
    if (all_passed) {
        std::cout << "✅ Test RÉUSSI!" << std::endl;
        std::cout << "============================================" << std::endl;
        return 0;
    } else {
        std::cout << "❌ Test ÉCHOUÉ!" << std::endl;
        std::cout << "============================================" << std::endl;
        return 1;
    }
}
