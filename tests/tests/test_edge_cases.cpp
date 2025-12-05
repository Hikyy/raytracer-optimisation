#include <iostream>
#include <cassert>
#include "ImageComparator.hpp"
#include "BenchmarkRunner.hpp"

// Fonction helper: exécute un test complet (render + mesure + validation)
bool runTestCase(const std::string& test_name,
                 const std::string& scene_path,
                 const std::string& output_path,
                 const std::string& reference_name,
                 int iterations = 2) {
    
    std::cout << "\n--- Test: " << test_name << " ---" << std::endl;
    std::cout << "Scène: " << scene_path << std::endl;
    
    // Lancer le benchmark avec N itérations
    auto metrics = BenchmarkRunner::runBenchmark(
        test_name,
        scene_path,
        output_path,
        iterations
    );
    
    // Vérifier succès du rendu
    if (!metrics.passed) {
        std::cerr << "❌ " << test_name << " benchmark ÉCHOUÉ: " 
                  << metrics.error_message << std::endl;
        return false;
    }
    
    // Afficher performance
    std::cout << "Temps: " << metrics.avg_time_seconds << "s, "
              << "Pixels/sec: " << metrics.samples_per_second << std::endl;
    
    // Sauvegarder métriques JSON
    std::string metrics_path = "../../tests/output/" + test_name + "_metrics.json";
    BenchmarkRunner::saveMetrics(metrics, metrics_path);
    
    // Comparaison d'image (si référence fournie)
    if (!reference_name.empty()) {
        bool image_ok = ImageComparator::compareWithReference(
            reference_name,
            output_path,
            2  // Tolérance ±2/255
        );
        
        if (!image_ok) {
            std::cerr << "❌ " << test_name << " comparaison d'image ÉCHOUÉE!" << std::endl;
            return false;
        }
    }
    
    std::cout << "✅ " << test_name << " RÉUSSI" << std::endl;
    return true;
}

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Cas Limites                   ===" << std::endl;
    std::cout << "============================================" << std::endl;
    
    bool all_passed = true;
    
    // TEST 1: Triangles (vérifie géométrie différente des sphères)
    all_passed &= runTestCase(
        "triangles",
        "../../scenes/two-triangles-on-plane.json",
        "../../tests/output/triangles.png",
        "triangles",  // Avec référence
        2
    );
    
    // TEST 2: Scène complexe (teste scalabilité avec centaines de sphères)
    all_passed &= runTestCase(
        "complex_scene",
        "../../scenes/sphere-galaxy-on-plane.json",
        "../../tests/output/complex_scene.png",
        "",  // SANS référence (trop long à valider)
        1    // 1 seule itération (scène lente: ~25s)
    );
    
    // TEST 3: Mesh 3D (vérifie chargement fichiers .obj)
    all_passed &= runTestCase(
        "mesh",
        "../../scenes/iso-sphere-on-plane.json",
        "../../tests/output/mesh.png",
        "mesh",  // Avec référence
        2
    );
    
    std::cout << "\n============================================" << std::endl;
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
