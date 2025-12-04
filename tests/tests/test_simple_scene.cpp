#include <iostream>
#include <cassert>
#include "ImageComparator.hpp"
#include "BenchmarkRunner.hpp"

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "=== Test: Simple Scene Rendering        ===" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    
    // Configuration
    std::string scene_path = "../../scenes/two-spheres-on-plane.json";
    std::string output_path = "../../tests/output/simple_scene.png";
    std::string test_name = "simple_scene";
    
    // Execute the benchmark
    std::cout << "Running benchmark for: " << scene_path << std::endl;
    auto metrics = BenchmarkRunner::runBenchmark(
        test_name, 
        scene_path, 
        output_path,
        3  // 3 iterations
    );
    
    // Check if benchmark passed
    if (!metrics.passed) {
        std::cerr << "❌ Benchmark FAILED: " << metrics.error_message << std::endl;
        return 1;
    }
    
    // Display the metrics
    std::cout << std::endl;
    std::cout << "--- Benchmark Results ---" << std::endl;
    std::cout << "Test:          " << test_name << std::endl;
    std::cout << "Avg Time:      " << metrics.avg_time_seconds << " s" << std::endl;
    std::cout << "Min Time:      " << metrics.min_time_seconds << " s" << std::endl;
    std::cout << "Max Time:      " << metrics.max_time_seconds << " s" << std::endl;
    std::cout << "Std Dev:       " << metrics.std_deviation << " s" << std::endl;
    std::cout << "Samples:       " << metrics.samples_rendered << std::endl;
    std::cout << "Samples/sec:   " << metrics.samples_per_second << std::endl;
    std::cout << std::endl;
    
    // Save the metrics
    std::string metrics_path = "../../tests/output/simple_scene_metrics.json";
    BenchmarkRunner::saveMetrics(metrics, metrics_path);
    
    // Compare the image with the reference
    std::cout << "--- Image Validation ---" << std::endl;
    bool image_ok = ImageComparator::compareWithReference(
        test_name, 
        output_path,
        2  // tolerance of 2/255 per channel
    );
    
    if (!image_ok) {
        std::cerr << "❌ Image comparison FAILED!" << std::endl;
        std::cerr << "The rendered image does not match the reference." << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "✅ Test PASSED!" << std::endl;
    std::cout << "============================================" << std::endl;
    
    return 0;
}
