#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;

json loadJson(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "❌ Error: File not found: " << path << std::endl;
        exit(1);
    }
    
    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: Invalid JSON in " << path << ": " << e.what() << std::endl;
        exit(1);
    }
    
    return j;
}

int compare(const std::string& baseline_path, const std::string& current_path) {
    json baseline = loadJson(baseline_path);
    json current = loadJson(current_path);
    
    // Extract metrics
    double b_time = baseline["metrics"]["avg_time_seconds"];
    double c_time = current["metrics"]["avg_time_seconds"];
    
    double b_samples_per_sec = baseline["metrics"]["samples_per_second"];
    double c_samples_per_sec = current["metrics"]["samples_per_second"];
    
    // Calculate improvements
    double speedup = b_time / c_time;
    double improvement = ((b_time - c_time) / b_time) * 100.0;
    
    // Print formatted report
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "Optimization Comparison Report" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Test: " << current["test_name"].get<std::string>() << std::endl;
    
    std::cout << "\nBaseline:" << std::endl;
    std::cout << "  Time:        " << std::fixed << std::setprecision(3) << b_time << "s" << std::endl;
    std::cout << "  Samples/sec: " << std::fixed << std::setprecision(0) << b_samples_per_sec << std::endl;
    
    std::cout << "\nCurrent:" << std::endl;
    std::cout << "  Time:        " << std::fixed << std::setprecision(3) << c_time << "s" << std::endl;
    std::cout << "  Samples/sec: " << std::fixed << std::setprecision(0) << c_samples_per_sec << std::endl;
    
    std::cout << "\nImprovement:" << std::endl;
    std::cout << "  Speedup:     " << std::fixed << std::setprecision(2) << speedup << "x ";
    std::cout << (speedup > 1 ? "faster" : "slower") << std::endl;
    std::cout << "  Change:      " << std::showpos << std::fixed << std::setprecision(1) << improvement << "%" << std::endl;
    
    // Determine status
    if (speedup < 0.95) {
        std::cout << "\n⚠️  WARNING: Performance REGRESSION detected!" << std::endl;
        std::cout << std::string(60, '=') << "\n" << std::endl;
        return 1;
    } else if (speedup > 1.2) {
        std::cout << "\n✅ Significant improvement!" << std::endl;
        std::cout << std::string(60, '=') << "\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n➡️  Performance stable" << std::endl;
        std::cout << std::string(60, '=') << "\n" << std::endl;
        return 0;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: compare_with_baseline <baseline.json> <current.json>" << std::endl;
        std::cout << "\nExample:" << std::endl;
        std::cout << "  ./compare_with_baseline \\" << std::endl;
        std::cout << "    ../../tests/baselines/v1.0_simple_scene.json \\" << std::endl;
        std::cout << "    ../../tests/output/simple_scene_metrics.json" << std::endl;
        return 1;
    }
    
    std::string baseline_path = argv[1];
    std::string current_path = argv[2];
    
    return compare(baseline_path, current_path);
}
