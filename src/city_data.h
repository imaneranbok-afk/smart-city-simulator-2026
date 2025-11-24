#ifndef CITY_DATA_H
#define CITY_DATA_H

#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>

// Types d'organisation urbaine
enum CityLayout {
    GRID_LAYOUT,
    CIRCULAR_LAYOUT,
    ORGANIC_LAYOUT
};

// Types de bâtiments futuristes
enum BuildingType {
    RESIDENTIAL_TOWER,
    COMMERCIAL_HUB,
    OFFICE_COMPLEX,
    SKYSCRAPER,
    ECO_BUILDING,
    TRANSPORT_HUB
};

// Structure pour les bâtiments futuristes
struct Building {
    Vector3 position;
    Vector3 size;
    Color mainColor;
    Color accentColor;
    BuildingType type;
    std::string name;
    bool hasGlowingWindows;
    bool hasHelipad;
    float glowIntensity;
};

// Structure pour les routes futuristes
struct RoadSegment {
    Vector3 start;
    Vector3 end;
    float width;
    Color roadColor;
    Color markingColor;
    bool hasCrosswalk;
    bool hasTrafficLight;
    bool hasGlowingMarkings;
    Vector3 trafficLightPos;
};

// Structure pour les feux tricolores futuristes
struct TrafficLight {
    Vector3 position;
    bool isRed;
    bool isYellow;
    bool isGreen;
    float timer;
    float glowEffect;
};

// Structure pour les passages piétons futuristes
struct Crosswalk {
    Vector3 position;
    float width;
    float length;
    bool isActive;
    float pulseIntensity;
};

// Structure principale
struct CityMap {
    std::vector<Building> buildings;
    std::vector<RoadSegment> roads;
    std::vector<TrafficLight> trafficLights;
    std::vector<Crosswalk> crosswalks;
    CityLayout layout;
};

// Constantes futuristes
const float ROAD_WIDTH = 25.0f; // Légèrement plus large
const float CELL_SIZE_MEDIUM = 100.0f; // Taille moyenne d'un bloc de ville
const float SIDEWALK_WIDTH = 4.0f;
const float LANE_WIDTH = 5.0f;

// Couleurs futuristes (Sombre et Néon)
const Color COLOR_ROAD_DARK = { 235, 235, 240, 255 };              // Gris route réaliste (foncé mais pas noir)
const Color COLOR_ROAD_NEON_MARKING = { 225, 225, 255, 255 };   // Bleu clair "fenêtre"
const Color COLOR_SIDEWALK_GLOW = { 255, 255, 255, 255 };       // Béton/gris clair urbain
const Color COLOR_CROSSWALK_NEON = { 235, 235, 240, 255 };      // Blanc/gris clair (comme la maquette)
const Color COLOR_GROUND_GRID = { 30, 35, 40, 255 };           // Gris-bleu foncé pour le sol


// Couleurs des bâtiments (Palette Néon Vibrante)
const Color COLOR_RESIDENTIAL_NEON = { 0, 220, 255, 255 };      // Cyan vibrant
const Color COLOR_COMMERCIAL_NEON  = { 255, 0, 220, 255 };      // Magenta vibrant
const Color COLOR_OFFICE_NEON      = { 240, 240, 240, 255 };    // Blanc/Gris très clair
const Color COLOR_SKYSCRAPER_NEON  = { 100, 60, 220, 255 };     // Indigo
const Color COLOR_ECO_NEON         = { 50, 255, 100, 255 };     // Vert néon
const Color COLOR_TRANSPORT_NEON   = { 255, 160, 0, 255 };     // Orange vif

// Couleurs d'accentuation (pour les lumières des fenêtres et les détails)
const Color COLOR_ACCENT_BLUE    = { 180, 220, 255, 255 };     // Bleu clair des fenêtres
const Color COLOR_ACCENT_YELLOW  = { 255, 240, 200, 255 };     // Jaune crème chaud
const Color COLOR_ACCENT_MAGENTA = { 230, 200, 255, 255 };     // Magenta doux
const Color COLOR_ACCENT_GREEN   = { 120, 200, 120, 255 };     // Vert pastel (arbres stylisés)  // Vert Vif

const Color COLOR_TRAFFIC_LIGHT_GLOW = { 255, 220, 100, 255 };

// Autres constantes du système
const float BUILDING_MIN_HEIGHT = 40.0f;
const float BUILDING_MAX_HEIGHT = 150.0f;


// Variables globales
// extern Vehicle emergencyVehicle; 
// extern Emergency currentEmergency;

#endif