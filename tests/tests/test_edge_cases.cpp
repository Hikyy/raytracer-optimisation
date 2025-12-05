#include <iostream>
#include <cassert>
#include "ImageComparator.hpp"
#include "ImageHasher.hpp"
#include "BenchmarkRunner.hpp"
#include "SceneRegistry.hpp"

/*
 * TEST: Cas Limites (EdgeCases)
 * Teste TOUTES les scènes pour s'assurer que le raytracer gère
 * correctement tous les types de géométrie et cas complexes
 */

int main(int argc, char** argv) {
    // Déterminer le mode
    bool use_multithread = true;
#ifndef USE_MULTITHREADING
    use_multithread = false;
#endif
    
    if (argc > 1 && std::string(argv[1]) == "--single-thread") {
        use_multithread = false;
    }
    
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Cas Limites                   ===" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "Mode: " << (use_multithread ? "Multi-Thread" : "Single-Thread") << std::endl;
    std::cout << std::endl;
    
    auto scenes = SceneRegistry::getAllScenes();
    bool all_passed = true;
    
    for (const auto& scene : scenes) {
        std::cout << "--- Test: " << scene.name << " ---" << std::endl;
        std::string output_path = "tests/output/" + scene.name + "_edge.png";
        
        auto metrics = BenchmarkRunner::runBenchmark(
            scene.name + "_edge",
            scene.scene_path,
            output_path,
            scene.iterations
        );
        
        if (!metrics.passed) {
            std::cerr << "❌ " << scene.name << " rendu échoué: " << metrics.error_message << std::endl;
            all_passed = false;
            continue;
        }
        
        std::cout << "Temps: " << metrics.avg_time_seconds << "s, Pixels/sec: " << metrics.samples_per_second << std::endl;
        
        if (scene.has_reference) {
            std::string ref_path = SceneRegistry::getReferencePathForMode(
                scene.reference_name,
                use_multithread
            );
            
            std::cout << "Pixel: ";
            bool pixel_ok = ImageComparator::compare(ref_path, output_path, 2);
            std::cout << "Hash: ";
            bool hash_ok = ImageHasher::compareByHash(ref_path, output_path);
            
            if (!pixel_ok) {
                std::cerr << "❌ " << scene.name << " comparaison ÉCHOUÉE!" << std::endl;
                all_passed = false;
            } else if (!hash_ok) {
                std::cout << "✅ " << scene.name << " OK (pixels), hash différent (multi-thread)" << std::endl;
            } else {
                std::cout << "✅ " << scene.name << " PARFAIT (pixels + hash)" << std::endl;
            }
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "============================================" << std::endl;
    if (all_passed) {
        std::cout << "✅ Tous les tests de cas limites RÉUSSIS!" << std::endl;
        std::cout << "============================================" << std::endl;
        return 0;
    } else {
        std::cout << "❌ Certains tests de cas limites ont ÉCHOUÉ!" << std::endl;
        std::cout << "============================================" << std::endl;
        return 1;
    }
}
