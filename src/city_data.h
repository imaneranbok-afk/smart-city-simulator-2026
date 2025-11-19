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
const float ROAD_WIDTH = 10.0f;
const float SIDEWALK_WIDTH = 3.0f;
const float LANE_WIDTH = 4.0f;

// Couleurs futuristes néon
const Color COLOR_ROAD_DARK = { 20, 20, 30, 255 };
const Color COLOR_ROAD_NEON_MARKING = { 0, 255, 255, 255 };    // Cyan néon
const Color COLOR_SIDEWALK_GLOW = { 100, 100, 150, 255 };
const Color COLOR_CROSSWALK_NEON = { 255, 50, 255, 255 };      // Magenta néon
const Color COLOR_TRAFFIC_LIGHT_GLOW = { 255, 200, 0, 255 };   // Or néon

// Couleurs des bâtiments futuristes
const Color COLOR_RESIDENTIAL_NEON = { 0, 150, 255, 255 };     // Bleu néon
const Color COLOR_COMMERCIAL_NEON = { 255, 100, 0, 255 };      // Orange néon
const Color COLOR_OFFICE_NEON = { 0, 255, 150, 255 };          // Vert néon
const Color COLOR_SKYSCRAPER_NEON = { 150, 0, 255, 255 };      // Violet néon
const Color COLOR_ECO_NEON = { 50, 255, 100, 255 };            // Vert éco
const Color COLOR_TRANSPORT_NEON = { 255, 255, 0, 255 };       // Jaune néon

// Accent colors
const Color COLOR_ACCENT_BLUE = { 0, 200, 255, 255 };
const Color COLOR_ACCENT_PURPLE = { 200, 0, 255, 255 };
const Color COLOR_ACCENT_ORANGE = { 255, 150, 0, 255 };

#endif