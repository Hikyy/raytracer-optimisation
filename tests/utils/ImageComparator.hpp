#pragma once
#include <string>
#include "Image.hpp"

class ImageComparator {
public:
    // Compare two images PNG with tolerance
    // tolerance = maximum acceptable difference per channel (0-255)
    static bool compare(const std::string& image1_path, 
                       const std::string& image2_path,
                       int tolerance = 2);
    
    // Compare a generated image with the reference image for current configuration
    // Utilise TestConfig pour déterminer la référence appropriée
    static bool compareWithReference(const std::string& test_name,
                                    const std::string& generated_path,
                                    int tolerance = 2);
    
    // Compare using hash (faster, exact match only)
    static bool compareWithReferenceByHash(const std::string& test_name,
                                          const std::string& generated_path);
    
private:
    static std::string getReferenceImagePath(const std::string& test_name);
};
