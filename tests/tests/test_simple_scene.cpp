#include <iostream>
#include <cassert>
#include "ImageComparator.hpp"
#include "BenchmarkRunner.hpp"

int main() {
    // En-tête du test
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Rendu Scène Simple            ===" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    
    // Configuration: chemins des fichiers
    std::string scene_path = "../../scenes/two-spheres-on-plane.json";
    std::string output_path = "../../tests/output/simple_scene.png";
    std::string test_name = "simple_scene";
    
    // ÉTAPE 1: Lancer le benchmark (3 itérations pour mesure précise)
    std::cout << "Running benchmark for: " << scene_path << std::endl;
    auto metrics = BenchmarkRunner::runBenchmark(
        test_name, 
        scene_path, 
        output_path,
        3  // 3 itérations pour avoir moyenne/min/max fiables
    );
    
    // Vérifier que le rendu a réussi
    if (!metrics.passed) {
        std::cerr << "❌ Benchmark ÉCHOUÉ: " << metrics.error_message << std::endl;
        return 1;
    }
    
    // ÉTAPE 2: Afficher les métriques de performance
    std::cout << std::endl;
    std::cout << "--- Résultats du Benchmark ---" << std::endl;
    std::cout << "Test:           " << test_name << std::endl;
    std::cout << "Temps moyen:    " << metrics.avg_time_seconds << " s" << std::endl;
    std::cout << "Temps min:      " << metrics.min_time_seconds << " s" << std::endl;
    std::cout << "Temps max:      " << metrics.max_time_seconds << " s" << std::endl;
    std::cout << "Écart-type:     " << metrics.std_deviation << " s" << std::endl;
    std::cout << "Échantillons:   " << metrics.samples_rendered << std::endl;
    std::cout << "Pixels/sec:     " << metrics.samples_per_second << std::endl;
    std::cout << std::endl;
    
    // ÉTAPE 3: Sauvegarder les métriques en JSON (pour comparaison avant/après)
    std::string metrics_path = "../../tests/output/simple_scene_metrics.json";
    BenchmarkRunner::saveMetrics(metrics, metrics_path);
    
    // ÉTAPE 4: Comparer l'image rendue avec la référence (validation visuelle)
    std::cout << "--- Validation de l'Image ---" << std::endl;
    bool image_ok = ImageComparator::compareWithReference(
        test_name, 
        output_path,
        2  // Tolérance: ±2/255 par canal (accepte micro-différences flottantes)
    );
    
    // Vérifier que l'image correspond à la référence
    if (!image_ok) {
        std::cerr << "❌ Comparaison d'image ÉCHOUÉE!" << std::endl;
        std::cerr << "L'image rendue ne correspond pas à la référence." << std::endl;
        return 1;
    }
    
    // Succès: performance mesurée + image validée
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "✅ Test RÉUSSI!" << std::endl;
    std::cout << "============================================" << std::endl;
    
    return 0;
}
