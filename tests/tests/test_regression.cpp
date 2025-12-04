#include <iostream>
#include "ImageComparator.hpp"
#include "BenchmarkRunner.hpp"
#include "lodepng.h"
#include <vector>

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Regression Detection          ===" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    
    std::cout << "This test demonstrates that the image comparison" << std::endl;
    std::cout << "system can detect visual regressions." << std::endl;
    std::cout << std::endl;
    
    // Configuration
    std::string scene_path = "../../scenes/two-spheres-on-plane.json";
    std::string output_normal = "../../tests/output/regression_normal.png";
    std::string output_modified = "../../tests/output/regression_modified.png";
    std::string test_name = "regression_test";
    
    // Step 1: Render the scene normally
    std::cout << "Step 1: Rendering scene normally..." << std::endl;
    auto metrics1 = BenchmarkRunner::runBenchmark(
        test_name + "_normal",
        scene_path,
        output_normal,
        1
    );
    
    if (!metrics1.passed) {
        std::cerr << "❌ Normal rendering FAILED!" << std::endl;
        return 1;
    }

    std::cout << "✅ Normal rendering completed" << std::endl;
    std::cout << std::endl;
    
    // Step 2: Load the normal image and modify it
    std::cout << "Step 2: Creating modified version..." << std::endl;
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, output_normal);
    
    if (error) {
        std::cerr << "❌ Failed to load normal image: " 
                  << lodepng_error_text(error) << std::endl;
        return 1;
    }
    
    // Intentionally modify the image: darken it by multiplying by 0.7
    for (size_t i = 0; i < image.size(); i += 4) {
        // Modify RGB channels, keep alpha
        image[i]     = static_cast<unsigned char>(image[i] * 0.7);     // R
        image[i + 1] = static_cast<unsigned char>(image[i + 1] * 0.7); // G
        image[i + 2] = static_cast<unsigned char>(image[i + 2] * 0.7); // B
    }
    
    // Save the modified image
    error = lodepng::encode(output_modified, image, width, height);
    if (error) {
        std::cerr << "❌ Failed to save modified image: " 
                  << lodepng_error_text(error) << std::endl;
        return 1;
    }
    std::cout << "✅ Modified version created (darkened by 30%)" << std::endl;
    std::cout << std::endl;
    
    // Step 3: Compare the images - they SHOULD be different
    std::cout << "Step 3: Comparing normal vs modified..." << std::endl;
    bool images_match = ImageComparator::compare(
        output_normal,
        output_modified,
        2  // tolerance
    );
    
    std::cout << std::endl;
    
    // Step 4: Verify that the comparison correctly detected the difference
    if (images_match) {
        std::cerr << "❌ REGRESSION TEST FAILED!" << std::endl;
        std::cerr << "The images should be different, but comparison says they match!" << std::endl;
        std::cerr << "This means the image comparison is not working correctly." << std::endl;
        return 1;
    } else {
        std::cout << "✅ REGRESSION TEST PASSED!" << std::endl;
        std::cout << "The comparison correctly detected the difference between images." << std::endl;
        std::cout << "This proves the system can detect visual regressions." << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "✅ Regression detection system validated!" << std::endl;
    std::cout << "============================================" << std::endl;
    
    return 0;
}
