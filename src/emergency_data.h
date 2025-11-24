#ifndef EMERGENCY_DATA_H
#define EMERGENCY_DATA_H

#include "raylib.h"
#include "city_data.h"
#include <vector>

// Structure pour représenter un véhicule prioritaire
struct Vehicle {
    Vector3 position;
    Vector3 direction;
    float speed;
    Color color;
    bool is_priority;
    int id;
};

// Structure pour représenter une urgence
struct Emergency {
    Vector3 location;
    bool is_active;
    float intensity;
    int target_building_index;
};

// Constantes pour les effets visuels (Sirènes classiques gardées pour la lisibilité)
const Color COLOR_SIREN_RED = { 255, 0, 0, 255 };      // Rouge de sirène
const Color COLOR_SIREN_BLUE = { 0, 0, 255, 255 };     // Bleu de sirène

// --- Mise à jour pour le Thème Neon (Magenta) ---
const Color COLOR_HIGHLIGHT_BUILDING = { 255, 0, 255, 255 }; // Magenta vif pour le surlignage
const Color COLOR_TRAIL = { 255, 0, 255, 100 };             // Traînée de Magenta/Fuchsia

// Variables globales
extern Vehicle emergencyVehicle;
extern Emergency currentEmergency;

#endif