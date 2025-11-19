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

// Constantes pour les effets visuels
const Color COLOR_SIREN_RED = { 255, 0, 0, 255 };
const Color COLOR_SIREN_BLUE = { 0, 0, 255, 255 };
const Color COLOR_HIGHLIGHT_BUILDING = { 255, 255, 0, 255 };
const Color COLOR_TRAIL = { 255, 165, 0, 100 };

// Variables globales
extern Vehicle emergencyVehicle;
extern Emergency currentEmergency;

#endif