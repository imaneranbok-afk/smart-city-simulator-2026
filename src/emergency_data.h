#ifndef EMERGENCY_DATA_H
#define EMERGENCY_DATA_H

#include "raylib.h"
#include "city_data.h"
#include <vector>

// Structure pour représenter un véhicule prioritaire
struct Vehicle {
    Vector3 position;       // Position actuelle du véhicule
    Vector3 direction;      // Vecteur de direction (pour l'orientation)
    float speed;            // Vitesse actuelle
    Color color;            // Couleur du véhicule (ex: rouge pour ambulance)
    bool is_priority;       // Est-ce un véhicule prioritaire (sirène active)
    int id;                 // Identifiant unique
};

// Structure pour représenter une urgence
struct Emergency {
    Vector3 location;       // Lieu de l'urgence (ex: bâtiment en feu)
    bool is_active;         // Statut de l'urgence
    float intensity;        // Intensité de l'urgence (pour les effets visuels)
    int target_building_index; // Index du bâtiment concerné dans CityMap::buildings
};

// Constantes pour les effets visuels
const Color COLOR_SIREN_RED = { 255, 0, 0, 255 };
const Color COLOR_SIREN_BLUE = { 0, 0, 255, 255 };
const Color COLOR_HIGHLIGHT_BUILDING = { 255, 255, 0, 255 }; // Jaune pour la surbrillance
const Color COLOR_TRAIL = { 255, 165, 0, 100 }; // Orange semi-transparent pour la traînée

// Variables globales pour la simulation (simplification pour l'exemple)
extern Vehicle emergencyVehicle;
extern Emergency currentEmergency;

#endif // EMERGENCY_DATA_H
