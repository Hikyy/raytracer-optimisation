#!/bin/bash
# Script de démonstration Valgrind SIMPLIFIÉ pour présentation
# =============================================================

clear

echo "╔════════════════════════════════════════════════════════╗"
echo "║                                                        ║"
echo "║        DÉMONSTRATION DES OPTIMISATIONS                ║"
echo "║        Comparaison AVANT/APRÈS avec Valgrind          ║"
echo "║                                                        ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""
echo "Appuyez sur ENTRÉE pour commencer..."
read

# ============================================================
# PARTIE 1: Vue d'ensemble
# ============================================================
clear
echo "┌────────────────────────────────────────────────────────┐"
echo "│  📊 RÉSUMÉ DES OPTIMISATIONS                          │"
echo "└────────────────────────────────────────────────────────┘"
echo ""
echo "  ⚙️  OPTIMISATIONS APPLIQUÉES:"
echo "  ───────────────────────────────────────────────────────"
echo ""
echo "  1️⃣  Suppression de countPrimes()"
echo "      → Fonction inutile qui consommait 46% du temps!"
echo ""
echo "  2️⃣  Utilisation de lengthSquared()"
echo "      → Évite les opérations sqrt() coûteuses"
echo ""
echo "  3️⃣  Pré-calculs dans Camera"
echo "      → Évite 1080 divisions répétées"
echo ""
echo "  4️⃣  Allocation sur la pile"
echo "      → Plus rapide que l'allocation tas"
echo ""
echo "  ───────────────────────────────────────────────────────"
echo ""
read -p "  Appuyez sur ENTRÉE pour voir les fichiers modifiés..."

# ============================================================
# PARTIE 2: Fichiers modifiés
# ============================================================
clear
echo "┌────────────────────────────────────────────────────────┐"
echo "│  📁 FICHIERS MODIFIÉS                                 │"
echo "└────────────────────────────────────────────────────────┘"
echo ""
echo "  4 fichiers ont été optimisés:"
echo "  ═══════════════════════════════════════════════════════"
echo ""
echo "  📄 src/rayscene/Sphere.cpp"
echo "     ├─ Suppression de countPrimes()          -31.6%"
echo "     └─ Utilisation de lengthSquared()        -2.1%"
echo ""
echo "  📄 src/raymath/Vector3.cpp"
echo "     ├─ Division optimisée (1 div + 3 mult)"
echo "     └─ normalize() avec 1 seul sqrt()"
echo ""
echo "  📄 src/rayscene/Scene.cpp"
echo "     └─ lengthSquared() dans closestIntersection()"
echo ""
echo "  📄 src/rayscene/Camera.cpp"
echo "     ├─ Pré-calcul de halfHeight               -1.1%"
echo "     └─ Allocation pile au lieu de tas"
echo ""
echo "  ───────────────────────────────────────────────────────"
echo "     💡 Tous les fichiers ont des commentaires en français!"
echo ""
echo ""
read -p "  Appuyez sur ENTRÉE pour voir les résultats chiffrés..."

# ============================================================
# PARTIE 2: Résultats chiffrés
# ============================================================
clear
echo "┌────────────────────────────────────────────────────────┐"
echo "│  📈 GAINS DE PERFORMANCE                              │"
echo "└────────────────────────────────────────────────────────┘"
echo ""
echo "  🔴 AVANT OPTIMISATION:"
echo "  ───────────────────────────────────────────────────────"
echo "     Temps d'exécution:  37.9 secondes"
echo "     Instructions:       43,981,627,909"
echo ""
echo ""
echo "  🟢 APRÈS OPTIMISATION:"
echo "  ───────────────────────────────────────────────────────"
echo "     Temps d'exécution:  25.7 secondes  ⬇️  -32.2%"
echo "     Instructions:       23,376,444,299  ⬇️  -46.9%"
echo ""
echo ""
echo "  💡 CONCLUSION:"
echo "  ───────────────────────────────────────────────────────"
echo "     ✅ 12.2 secondes économisées!"
echo "     ✅ ~20 MILLIARDS d'instructions en moins!"
echo "     ✅ Code 1.5× plus rapide!"
echo ""
echo ""
read -p "  Appuyez sur ENTRÉE pour voir les détails Valgrind..."

# ============================================================
# PARTIE 3: Profil AVANT (simplifié)
# ============================================================
clear
echo "┌────────────────────────────────────────────────────────┐"
echo "│  🔍 PROFIL VALGRIND - AVANT OPTIMISATION              │"
echo "└────────────────────────────────────────────────────────┘"
echo ""
echo "  Fichier analysé: callgrind.out.617"
echo ""

cd /app
callgrind_annotate callgrind.out.617 2>&1 | head -30 | tail -15

echo ""
echo "  ⚠️  PROBLÈME IDENTIFIÉ:"
echo "  ───────────────────────────────────────────────────────"
echo "     → Sphere::countPrimes consomme 46% du temps!"
echo "     → Cette fonction est COMPLÈTEMENT INUTILE"
echo "     → Elle est appelée des MILLIERS de fois"
echo ""
read -p "  Appuyez sur ENTRÉE pour voir le profil APRÈS..."

# ============================================================
# PARTIE 4: Profil APRÈS (simplifié)
# ============================================================
clear
echo "┌────────────────────────────────────────────────────────┐"
echo "│  🔍 PROFIL VALGRIND - APRÈS OPTIMISATION              │"
echo "└────────────────────────────────────────────────────────┘"
echo ""
echo "  Fichier analysé: callgrind.out.5322"
echo ""

callgrind_annotate callgrind.out.5322 2>&1 | head -30 | tail -15

echo ""
echo "  ✅ RÉSULTAT:"
echo "  ───────────────────────────────────────────────────────"
echo "     → countPrimes a COMPLÈTEMENT DISPARU!"
echo "     → Le temps est maintenant réparti normalement"
echo "     → Aucune fonction ne domine le profil"
echo ""
read -p "  Appuyez sur ENTRÉE pour le récapitulatif final..."

# ============================================================
# PARTIE 5: Récapitulatif final
# ============================================================
clear
echo "╔════════════════════════════════════════════════════════╗"
echo "║                                                        ║"
echo "║        ✅ OPTIMISATIONS RÉUSSIES!                      ║"
echo "║                                                        ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""
echo ""
echo "  📊 RÉSULTATS FINAUX:"
echo "  ════════════════════════════════════════════════════════"
echo ""
echo "     Gain de temps:         -32.2%  (37.9s → 25.7s)"
echo "     Gain d'instructions:   -46.9%  (~20 milliards)"
echo ""
echo ""
echo "  🎯 OPTIMISATIONS CLÉS:"
echo "  ════════════════════════════════════════════════════════"
echo ""
echo "     1. Suppression countPrimes()     -31.6%"
echo "     2. lengthSquared() dans Sphere   -2.1%"
echo "     3. Pré-calculs Camera            -1.1%"
echo "     4. Optimisations Vector3         ~0%"
echo ""
echo ""
echo "  💡 LEÇON APPRISE:"
echo "  ════════════════════════════════════════════════════════"
echo ""
echo "     → Toujours PROFILER avant d'optimiser!"
echo "     → Un petit changement peut avoir un GROS impact"
echo "     → 46% du temps perdu dans UNE fonction inutile!"
echo ""
echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║           DÉMONSTRATION TERMINÉE                       ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""
