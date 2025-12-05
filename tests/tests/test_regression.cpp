#include <iostream>
#include <cassert>
#include "ImageComparator.hpp"
#include "ImageHasher.hpp"
#include "BenchmarkRunner.hpp"
#include "SceneRegistry.hpp"

/*
 * TEST DE RÉGRESSION selon les exigences du prof:
 * "Une fois votre optimisation mise en œuvre, vous devez tester votre code!
 *  9 fois sur 10, une optimisation casse le code existant!"
 * 
 * Ce test vérifie qu'APRÈS optimisation, le raytracer produit
 * toujours des images CORRECTES sur TOUTES les scènes.
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
    std::cout << "=== Test: Régression Post-Optimisation  ===" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "Mode: " << (use_multithread ? "Multi-Thread" : "Single-Thread") << std::endl;
    std::cout << std::endl;
    std::cout << "Objectif: Vérifier que les optimisations n'ont PAS" << std::endl;
    std::cout << "          cassé la qualité visuelle du rendu" << std::endl;
    std::cout << std::endl;
    
    auto scenes = SceneRegistry::getAllScenes();
    bool all_passed = true;
    
    for (const auto& scene : scenes) {
        std::cout << "--- Test: " << scene.name << " ---" << std::endl;
        std::string output_path = "tests/output/" + scene.name + "_regression.png";
        
        auto metrics = BenchmarkRunner::runBenchmark(
            scene.name + "_regression",
            scene.scene_path,
            output_path,
            1  // 1 itération suffit pour régression
        );
        
        if (!metrics.passed) {
            std::cerr << "❌ Rendu ÉCHOUÉ: " << metrics.error_message << std::endl;
            all_passed = false;
            continue;
        }
        
        if (scene.has_reference) {
            std::string ref_path = SceneRegistry::getReferencePathForMode(
                scene.reference_name,
                use_multithread
            );
            
            bool pixel_ok = ImageComparator::compare(ref_path, output_path, 2);
            bool hash_ok = ImageHasher::compareByHash(ref_path, output_path);
            
            if (!pixel_ok) {
                std::cerr << "❌ RÉGRESSION DÉTECTÉE sur " << scene.name << "!" << std::endl;
                std::cerr << "   → L'optimisation a cassé le rendu" << std::endl;
                all_passed = false;
            } else if (!hash_ok) {
                std::cout << "✅ " << scene.name << ": OK (pixels), hash différent" << std::endl;
            } else {
                std::cout << "✅ " << scene.name << ": PARFAIT (pixels + hash)" << std::endl;
            }
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "============================================" << std::endl;
    if (all_passed) {
        std::cout << "✅ AUCUNE RÉGRESSION DÉTECTÉE!" << std::endl;
        std::cout << "   Les optimisations n'ont rien cassé" << std::endl;
        std::cout << "============================================" << std::endl;
        return 0;
    } else {
        std::cout << "❌ RÉGRESSION(S) DÉTECTÉE(S)!" << std::endl;
        std::cout << "   Une ou plusieurs optimisations ont" << std::endl;
        std::cout << "   cassé la qualité du rendu" << std::endl;
        std::cout << "============================================" << std::endl;
        return 1;
    }
}
