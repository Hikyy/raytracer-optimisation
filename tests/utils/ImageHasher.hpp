#pragma once
#include <string>
#include <cstdint>

/**
 * Utilitaire pour calculer des hash d'images
 * Permet de comparer rapidement si deux images sont identiques
 */
class ImageHasher {
public:
    /**
     * Calcule un hash SHA-256 simple d'une image PNG
     * @param image_path Chemin vers l'image PNG
     * @return Hash hexadécimal (64 caractères) ou chaîne vide si erreur
     */
    static std::string computeImageHash(const std::string& image_path);
    
    /**
     * Compare deux images par leur hash
     * @param image1_path Chemin vers première image
     * @param image2_path Chemin vers deuxième image
     * @return true si les hash correspondent (images identiques)
     */
    static bool compareByHash(const std::string& image1_path, const std::string& image2_path);
    
    /**
     * Sauvegarde le hash d'une image dans un fichier .hash
     * @param image_path Chemin vers l'image
     * @param hash_path Chemin où sauvegarder le hash (optionnel, par défaut: image_path + ".hash")
     * @return true si succès
     */
    static bool saveHash(const std::string& image_path, const std::string& hash_path = "");
    
    /**
     * Charge un hash depuis un fichier .hash
     * @param hash_path Chemin vers le fichier hash
     * @return Hash chargé ou chaîne vide si erreur
     */
    static std::string loadHash(const std::string& hash_path);

private:
    // Hash simple basé sur les données de l'image
    static uint64_t simpleHash(const unsigned char* data, size_t size);
    static std::string hashToHex(uint64_t hash);
};
