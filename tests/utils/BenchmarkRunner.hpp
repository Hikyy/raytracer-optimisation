#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "Scene.hpp"
#include "Camera.hpp"
#include "Image.hpp"

struct BenchmarkMetrics {
    std::string test_name;
    double avg_time_seconds;
    double min_time_seconds;
    double max_time_seconds;
    double std_deviation;
    int samples_rendered;  // width * height
    double samples_per_second;
    bool passed;
    std::string error_message;
};

class BenchmarkRunner {
public:
    // Execute a rendering test with multiple iterations
    static BenchmarkMetrics runBenchmark(
        const std::string& test_name,
        const std::string& scene_path,
        const std::string& output_path,
        int iterations = 3
    );
    
    // Save metrics to JSON file
    static void saveMetrics(const BenchmarkMetrics& metrics,
                          const std::string& output_path);
    
    // Compare with baseline metrics
    static void compareWithBaseline(const BenchmarkMetrics& current,
                                   const std::string& baseline_path);
    
private:
    static double computeStdDev(const std::vector<double>& times, double avg);
};
