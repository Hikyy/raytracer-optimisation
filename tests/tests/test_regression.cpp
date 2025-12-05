#include <iostream>
#include "ImageComparator.hpp"
#include "BenchmarkRunner.hpp"
#include "lodepng.h"
#include <vector>

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Détection de Régression       ===" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    
    // Objectif: TESTER le système de détection lui-même
    std::cout << "Ce test démontre que le système de comparaison d'images" << std::endl;
    std::cout << "peut détecter les régressions visuelles." << std::endl;
    std::cout << std::endl;
    
    // Configuration
    std::string scene_path = "../../scenes/two-spheres-on-plane.json";
    std::string output_normal = "../../tests/output/regression_normal.png";
    std::string output_modified = "../../tests/output/regression_modified.png";
    std::string test_name = "regression_test";
    
    // ÉTAPE 1: Rendre la scène normalement (image de référence)
    std::cout << "Étape 1: Rendu de la scène normale..." << std::endl;
    auto metrics1 = BenchmarkRunner::runBenchmark(
        test_name + "_normal",
        scene_path,
        output_normal,
        1
    );
    
    if (!metrics1.passed) {
        std::cerr << "❌ Rendu normal ÉCHOUÉ!" << std::endl;
        return 1;
    }

    std::cout << "✅ Rendu normal terminé" << std::endl;
    std::cout << std::endl;
    
    // ÉTAPE 2: Charger l'image et la modifier VOLONTAIREMENT
    std::cout << "Étape 2: Création de la version modifiée..." << std::endl;
    std::vector<unsigned char> image;  // Tableau de pixels [R,G,B,A,R,G,B,A,...]
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, output_normal);
    
    if (error) {
        std::cerr << "❌ Échec du chargement de l'image normale: " 
                  << lodepng_error_text(error) << std::endl;
        return 1;
    }
    
    // Modifier INTENTIONNELLEMENT: assombrir de 30% (simulation de bug)
    for (size_t i = 0; i < image.size(); i += 4) {
        // Modifier RGB (× 0.7), garder Alpha intact
        image[i]     = static_cast<unsigned char>(image[i] * 0.7);     // R
        image[i + 1] = static_cast<unsigned char>(image[i + 1] * 0.7); // G
        image[i + 2] = static_cast<unsigned char>(image[i + 2] * 0.7); // B
        // image[i + 3] = Alpha non modifié
    }
    
    // Sauvegarder l'image modifiée
    error = lodepng::encode(output_modified, image, width, height);
    if (error) {
        std::cerr << "❌ Échec de la sauvegarde de l'image modifiée: " 
                  << lodepng_error_text(error) << std::endl;
        return 1;
    }
    std::cout << "✅ Version modifiée créée (assombrie de 30%)" << std::endl;
    std::cout << std::endl;
    
    // ÉTAPE 3: Comparer les 2 images (normale vs modifiée)
    std::cout << "Étape 3: Comparaison normale vs modifiée..." << std::endl;
    bool images_match = ImageComparator::compare(
        output_normal,    // Image normale
        output_modified,  // Image assombrie de 30%
        2  // Tolérance ±2/255
    );
    
    std::cout << std::endl;
    
    // ÉTAPE 4: Vérifier que le comparateur A DÉTECTÉ la différence
    // LOGIQUE INVERSÉE: on VEUT que images_match = false!
    if (images_match) {
        // MAUVAIS! Le comparateur n'a PAS détecté la modification
        std::cerr << "❌ TEST DE RÉGRESSION ÉCHOUÉ!" << std::endl;
        std::cerr << "Les images devraient être différentes, mais la comparaison dit qu'elles correspondent!" << std::endl;
        std::cerr << "Cela signifie que la comparaison d'images ne fonctionne pas correctement." << std::endl;
        return 1;
    } else {
        // BON! Le comparateur a détecté la différence → il fonctionne
        std::cout << "✅ TEST DE RÉGRESSION RÉUSSI!" << std::endl;
        std::cout << "La comparaison a correctement détecté la différence entre les images." << std::endl;
        std::cout << "Cela prouve que le système peut détecter les régressions visuelles." << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "✅ Système de détection de régression validé!" << std::endl;
    std::cout << "============================================" << std::endl;
    
    return 0;
}
