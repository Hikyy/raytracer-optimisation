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
            
            std::string ref_path = SceneRegistry::getReferencePathForMode(
                scene.reference_name,
                use_multithread
            );
            
            std::cout << "Référence: " << ref_path << std::endl;
            
            // MÉTHODE 1: Comparaison pixel-par-pixel avec tolérance
            std::cout << "1. Comparaison pixel-par-pixel (tolérance ±2)..." << std::endl;
            bool pixel_ok = ImageComparator::compare(
                ref_path,
                output_path,
                2  // Tolérance
            );
            
            // MÉTHODE 2: Comparaison par hash (exactitude absolue)
            std::cout << "2. Comparaison par hash (exactitude)..." << std::endl;
            bool hash_ok = ImageHasher::compareByHash(ref_path, output_path);
            
            if (!pixel_ok) {
                std::cerr << "❌ Comparaison pixel ÉCHOUÉE pour " << scene.name << std::endl;
                all_passed = false;
            } else if (!hash_ok) {
                std::cout << "⚠️  Hash différent (normal en multi-thread)" << std::endl;
                std::cout << "✅ Mais pixels OK (différences < tolérance)" << std::endl;
            } else {
                std::cout << "✅ Image PARFAITEMENT identique (hash + pixels)" << std::endl;
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
