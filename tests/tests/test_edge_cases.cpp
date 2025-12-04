#include <iostream>
#include <cassert>
#include "ImageComparator.hpp"
#include "BenchmarkRunner.hpp"

// Helper function to run a single test case
bool runTestCase(const std::string& test_name,
                 const std::string& scene_path,
                 const std::string& output_path,
                 const std::string& reference_name,
                 int iterations = 2) {
    
    std::cout << "\n--- Testing: " << test_name << " ---" << std::endl;
    std::cout << "Scene: " << scene_path << std::endl;
    
    // Run benchmark
    auto metrics = BenchmarkRunner::runBenchmark(
        test_name,
        scene_path,
        output_path,
        iterations
    );
    
    if (!metrics.passed) {
        std::cerr << "❌ " << test_name << " benchmark FAILED: " 
                  << metrics.error_message << std::endl;
        return false;
    }
    
    std::cout << "Time: " << metrics.avg_time_seconds << "s, "
              << "Samples/sec: " << metrics.samples_per_second << std::endl;
    
    // Save metrics
    std::string metrics_path = "../../tests/output/" + test_name + "_metrics.json";
    BenchmarkRunner::saveMetrics(metrics, metrics_path);
    
    // Compare with reference if one is provided
    if (!reference_name.empty()) {
        bool image_ok = ImageComparator::compareWithReference(
            reference_name,
            output_path,
            2
        );
        
        if (!image_ok) {
            std::cerr << "❌ " << test_name << " image comparison FAILED!" << std::endl;
            return false;
        }
    }
    
    std::cout << "✅ " << test_name << " PASSED" << std::endl;
    return true;
}

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Edge Cases                    ===" << std::endl;
    std::cout << "============================================" << std::endl;
    
    bool all_passed = true;
    
    // Test 1: Triangle mesh
    all_passed &= runTestCase(
        "triangles",
        "../../scenes/two-triangles-on-plane.json",
        "../../tests/output/triangles.png",
        "triangles",
        2
    );
    
    // Test 2: Complex scene with many objects
    all_passed &= runTestCase(
        "complex_scene",
        "../../scenes/sphere-galaxy-on-plane.json",
        "../../tests/output/complex_scene.png",
        "",  // No reference image for now
        1    // Only 1 iteration since it's slow
    );
    
    // Test 3: 3D mesh (iso-sphere)
    all_passed &= runTestCase(
        "mesh",
        "../../scenes/iso-sphere-on-plane.json",
        "../../tests/output/mesh.png",
        "mesh",
        2
    );
    
    std::cout << "\n============================================" << std::endl;
    if (all_passed) {
        std::cout << "✅ All edge case tests PASSED!" << std::endl;
        std::cout << "============================================" << std::endl;
        return 0;
    } else {
        std::cout << "❌ Some edge case tests FAILED!" << std::endl;
        std::cout << "============================================" << std::endl;
        return 1;
    }
}
