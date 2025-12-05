#include "ImageHasher.hpp"
#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

// Hash FNV-1a simple pour les données d'image
uint64_t ImageHasher::simpleHash(const unsigned char* data, size_t size) {
    const uint64_t FNV_PRIME = 0x100000001b3ULL;
    const uint64_t FNV_OFFSET = 0xcbf29ce484222325ULL;
    
    uint64_t hash = FNV_OFFSET;
    for (size_t i = 0; i < size; i++) {
        hash ^= data[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

std::string ImageHasher::hashToHex(uint64_t hash) {
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

std::string ImageHasher::computeImageHash(const std::string& image_path) {
    // Charger l'image
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, image_path);
    
    if (error) {
        std::cerr << "Erreur lors du chargement pour hash (" << image_path << "): "
                  << lodepng_error_text(error) << std::endl;
        return "";
    }
    
    // Calculer hash des données de l'image
    uint64_t hash = simpleHash(image.data(), image.size());
    return hashToHex(hash);
}

bool ImageHasher::compareByHash(const std::string& image1_path, const std::string& image2_path) {
    std::string hash1 = computeImageHash(image1_path);
    std::string hash2 = computeImageHash(image2_path);
    
    if (hash1.empty() || hash2.empty()) {
        return false;
    }
    
    bool match = (hash1 == hash2);
    
    if (match) {
        std::cout << "✅ Hash correspondent: " << hash1 << std::endl;
    } else {
        std::cout << "❌ Hash différents:" << std::endl;
        std::cout << "   Image 1: " << hash1 << std::endl;
        std::cout << "   Image 2: " << hash2 << std::endl;
    }
    
    return match;
}

bool ImageHasher::saveHash(const std::string& image_path, const std::string& hash_path) {
    std::string hash = computeImageHash(image_path);
    if (hash.empty()) {
        return false;
    }
    
    std::string output_path = hash_path.empty() ? (image_path + ".hash") : hash_path;
    
    std::ofstream file(output_path);
    if (!file.is_open()) {
        std::cerr << "Impossible d'écrire le hash: " << output_path << std::endl;
        return false;
    }
    
    file << hash << std::endl;
    file.close();
    
    std::cout << "Hash sauvegardé: " << output_path << " (" << hash << ")" << std::endl;
    return true;
}

std::string ImageHasher::loadHash(const std::string& hash_path) {
    std::ifstream file(hash_path);
    if (!file.is_open()) {
        return "";
    }
    
    std::string hash;
    std::getline(file, hash);
    file.close();
    
    return hash;
}
