#include "BenchmarkRunner.hpp"
#include "SceneLoader.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using json = nlohmann::json;

double BenchmarkRunner::computeStdDev(const std::vector<double>& times, double avg) {
    if (times.size() <= 1) return 0.0;
    
    double sum_sq_diff = 0.0;
    for (double time : times) {
        double diff = time - avg;
        sum_sq_diff += diff * diff;
    }
    
    return std::sqrt(sum_sq_diff / (times.size() - 1));
}

BenchmarkMetrics BenchmarkRunner::runBenchmark(
    const std::string& test_name,
    const std::string& scene_path,
    const std::string& output_path,
    int iterations) {
    
    BenchmarkMetrics metrics;
    metrics.test_name = test_name;
    metrics.passed = false;
    
    try {
        // Load the scene
        std::cout << "Loading scene: " << scene_path << std::endl;
        auto [scene, camera, image] = SceneLoader::Load(scene_path);
        
        if (!scene || !camera || !image) {
            metrics.error_message = "Failed to load scene";
            return metrics;
        }
        
        metrics.samples_rendered = image->width * image->height;
        
        std::vector<double> times;
        times.reserve(iterations);
        
        std::cout << "Running " << iterations << " iterations..." << std::endl;
        
        // Run rendering iterations
        for (int i = 0; i < iterations; i++) {
            std::cout << "  Iteration " << (i + 1) << "/" << iterations << "... ";
            
            auto begin = std::chrono::high_resolution_clock::now();
            camera->render(*image, *scene);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            double time_seconds = elapsed.count() * 1e-9;
            times.push_back(time_seconds);
            
            std::cout << time_seconds << "s" << std::endl;
        }
        
        // Calculate statistics
        metrics.min_time_seconds = *std::min_element(times.begin(), times.end());
        metrics.max_time_seconds = *std::max_element(times.begin(), times.end());
        
        double sum = 0.0;
        for (double time : times) {
            sum += time;
        }
        metrics.avg_time_seconds = sum / times.size();
        metrics.std_deviation = computeStdDev(times, metrics.avg_time_seconds);
        
        // Calculate samples per second
        double total_samples = metrics.samples_rendered * iterations;
        metrics.samples_per_second = total_samples / sum;
        
        // Save the final rendered image
        std::cout << "Saving image to: " << output_path << std::endl;
        std::string output_copy = output_path; // writeFile requires non-const reference
        image->writeFile(output_copy);
        
        metrics.passed = true;
        
        // Cleanup
        delete scene;
        delete camera;
        delete image;
        
    } catch (const std::exception& e) {
        metrics.error_message = std::string("Exception: ") + e.what();
        std::cerr << "Error during benchmark: " << metrics.error_message << std::endl;
    }
    
    return metrics;
}

void BenchmarkRunner::saveMetrics(const BenchmarkMetrics& metrics,
                                 const std::string& output_path) {
    json j;
    j["test_name"] = metrics.test_name;
    j["metrics"]["avg_time_seconds"] = metrics.avg_time_seconds;
    j["metrics"]["min_time_seconds"] = metrics.min_time_seconds;
    j["metrics"]["max_time_seconds"] = metrics.max_time_seconds;
    j["metrics"]["std_deviation"] = metrics.std_deviation;
    j["metrics"]["samples_rendered"] = metrics.samples_rendered;
    j["metrics"]["samples_per_second"] = metrics.samples_per_second;
    j["passed"] = metrics.passed;
    
    if (!metrics.error_message.empty()) {
        j["error_message"] = metrics.error_message;
    }
    
    std::ofstream file(output_path);
    if (file.is_open()) {
        file << j.dump(2);  // Pretty print with 2-space indentation
        file.close();
        std::cout << "Metrics saved to: " << output_path << std::endl;
    } else {
        std::cerr << "Failed to save metrics to: " << output_path << std::endl;
    }
}

void BenchmarkRunner::compareWithBaseline(const BenchmarkMetrics& current,
                                         const std::string& baseline_path) {
    try {
        std::ifstream file(baseline_path);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open baseline file: " << baseline_path << std::endl;
            return;
        }
        
        json baseline_json;
        file >> baseline_json;
        file.close();
        
        double baseline_time = baseline_json["metrics"]["avg_time_seconds"];
        double current_time = current.avg_time_seconds;
        
        double speedup = baseline_time / current_time;
        double improvement = ((baseline_time - current_time) / baseline_time) * 100.0;
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "Performance Comparison" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Baseline time: " << baseline_time << "s" << std::endl;
        std::cout << "Current time:  " << current_time << "s" << std::endl;
        std::cout << "Speedup:       " << speedup << "x" << std::endl;
        std::cout << "Improvement:   " << (improvement >= 0 ? "+" : "") << improvement << "%" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        if (speedup < 0.95) {
            std::cout << "⚠️  WARNING: Performance REGRESSION detected!" << std::endl;
        } else if (speedup > 1.2) {
            std::cout << "✅ Significant improvement!" << std::endl;
        } else {
            std::cout << "➡️  Performance stable" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error comparing with baseline: " << e.what() << std::endl;
    }
}
