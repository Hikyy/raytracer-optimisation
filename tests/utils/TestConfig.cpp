#include "TestConfig.hpp"
#include <fstream>

std::string TestConfig::getConfigSuffix() {
#ifdef USE_MULTITHREADING
    return "_multithread";
#else
    return "_singlethread";
#endif
}

std::string TestConfig::getConfigName() {
#ifdef USE_MULTITHREADING
    return "Multi-Thread";
#else
    return "Single-Thread";
#endif
}

std::string TestConfig::getReferencePathForConfig(const std::string& test_name) {
    std::string suffix = getConfigSuffix();
    std::string with_suffix = "../../tests/references/" + test_name + suffix + ".png";
    
    // Vérifier si la référence avec suffixe existe
    std::ifstream test_file(with_suffix);
    if (test_file.good()) {
        return with_suffix;
    }
    
    // Fallback: utiliser la référence sans suffixe (compatibilité)
    return "../../tests/references/" + test_name + ".png";
}

bool TestConfig::hasReferenceForConfig(const std::string& test_name) {
    std::string ref_path = getReferencePathForConfig(test_name);
    std::ifstream file(ref_path);
    return file.good();
}
