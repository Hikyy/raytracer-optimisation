#include "ImageComparator.hpp"
#include "lodepng.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

std::string ImageComparator::getReferenceImagePath(const std::string& test_name) {
    return "../../tests/references/" + test_name + ".png";
}

bool ImageComparator::compare(const std::string& image1_path, 
                              const std::string& image2_path,
                              int tolerance) {
    // Load first image
    std::vector<unsigned char> image1;
    unsigned width1, height1;
    unsigned error1 = lodepng::decode(image1, width1, height1, image1_path);
    
    if (error1) {
        std::cerr << "Erreur lors du chargement de l'image1 (" << image1_path << "): " 
                  << lodepng_error_text(error1) << std::endl;
        return false;
    }
    
    // Load second image
    std::vector<unsigned char> image2;
    unsigned width2, height2;
    unsigned error2 = lodepng::decode(image2, width2, height2, image2_path);
    
    if (error2) {
        std::cerr << "Erreur lors du chargement de l'image2 (" << image2_path << "): " 
                  << lodepng_error_text(error2) << std::endl;
        return false;
    }
    
    // Check dimensions match
    if (width1 != width2 || height1 != height2) {
        std::cerr << "Les dimensions des images ne correspondent pas: " 
                  << width1 << "x" << height1 << " vs " 
                  << width2 << "x" << height2 << std::endl;
        return false;
    }
    
    // Compare pixel by pixel
    size_t num_pixels = width1 * height1;
    size_t num_values = num_pixels * 4; // RGBA
    int max_diff = 0;
    size_t diff_count = 0;
    
    for (size_t i = 0; i < num_values; i++) {
        int diff = std::abs(static_cast<int>(image1[i]) - static_cast<int>(image2[i]));
        
        if (diff > tolerance) {
            diff_count++;
            if (diff > max_diff) {
                max_diff = diff;
            }
        }
    }
    
    if (diff_count > 0) {
        double diff_percentage = (100.0 * diff_count) / num_values;
        std::cerr << "Les images diffèrent: " << diff_count << " valeurs (" 
                  << diff_percentage << "%) dépassent la tolérance " << tolerance 
                  << ", diff max: " << max_diff << std::endl;
        return false;
    }
    
    std::cout << "✅ Les images correspondent (tolérance " << tolerance << ")" << std::endl;
    return true;
}

bool ImageComparator::compareWithReference(const std::string& test_name,
                                           const std::string& generated_path,
                                           int tolerance) {
    std::string reference_path = getReferenceImagePath(test_name);
    std::cout << "Comparaison avec la référence: " << reference_path << std::endl;
    return compare(reference_path, generated_path, tolerance);
}
