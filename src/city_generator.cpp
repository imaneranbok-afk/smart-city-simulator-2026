#include "city_generator.h"
#include "raymath.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>

// --- Paramètres de Hauteur des Bâtiments ---
// J'ai réduit la hauteur maximale pour éviter les gratte-ciels très hauts.
const float MAX_BUILDING_HEIGHT_GRID = 30.0f; // Anciennement 60.0f
const float MIN_BUILDING_HEIGHT_GRID = 15.0f; // Anciennement 20.0f
const float MAX_BUILDING_HEIGHT_CIRCULAR = 40.0f; // Anciennement 70.0f
const float MIN_BUILDING_HEIGHT_CIRCULAR = 10.0f; // Anciennement 15.0f
const float MAX_BUILDING_HEIGHT_ORGANIC = 30.0f; // Anciennement 50.0f
const float MIN_BUILDING_HEIGHT_ORGANIC = 8.0f; // Anciennement 10.0f


float GetRandomFloat(float min, float max) {
    // S'assurer que rand() est initialisé (fait dans main.cpp normalement)
    return min + (float)rand() / (float)RAND_MAX * (max - min);
}

BuildingType GetRandomFuturisticBuildingType() {
    int random = rand() % 100;
    if (random < 30) return RESIDENTIAL_TOWER;
    if (random < 50) return COMMERCIAL_HUB;
    if (random < 70) return OFFICE_COMPLEX;
    if (random < 85) return SKYSCRAPER;
    if (random < 95) return ECO_BUILDING;
    return TRANSPORT_HUB;
}

Color GetFuturisticBuildingColor(BuildingType type) {
    switch (type) {
        case RESIDENTIAL_TOWER: return COLOR_RESIDENTIAL_NEON;
        case COMMERCIAL_HUB: return COLOR_COMMERCIAL_NEON;
        case OFFICE_COMPLEX: return COLOR_OFFICE_NEON;
        case SKYSCRAPER: return COLOR_SKYSCRAPER_NEON;
        case ECO_BUILDING: return COLOR_ECO_NEON;
        case TRANSPORT_HUB: return COLOR_TRANSPORT_NEON;
        default: return COLOR_RESIDENTIAL_NEON;
    }
}

Color GetFuturisticAccentColor(BuildingType type) {
    switch (type) {
        case RESIDENTIAL_TOWER: return COLOR_ACCENT_BLUE;
        case COMMERCIAL_HUB: return COLOR_ACCENT_MAGENTA;
        // case OFFICE_COMPLEX: return COLOR_ACCENT_CYAN;
        // case SKYSCRAPER: return COLOR_ACCENT_LIME;
        case ECO_BUILDING: return COLOR_ACCENT_GREEN;
        case TRANSPORT_HUB: return COLOR_ACCENT_YELLOW;
        default: return COLOR_ACCENT_BLUE;
    }
}

// Fonction utilitaire pour ajouter des feux de signalisation sur une route
void GenerateHolographicTrafficLights(CityMap& city) {
    // Logique omise pour la concision (elle est déjà dans votre code original si nécessaire)
}

// Fonction utilitaire pour ajouter des passages piétons lumineux sur une route
void GenerateGlowingCrosswalks(CityMap& city) {
    // Logique omise pour la concision (elle est déjà dans votre code original si nécessaire)
}


// Génération de la ville en grille (GRID_LAYOUT)
CityMap GenerateGridLayout(int grid_size, float cell_size) {
    CityMap city;
    city.layout = GRID_LAYOUT;
    float map_size = grid_size * cell_size;

    // Génération des routes
    for (int i = 0; i <= grid_size; ++i) {
        // Routes horizontales (Z constant)
        RoadSegment h_road = {};
        h_road.start = (Vector3){ -map_size / 2, 0.0f, -map_size / 2 + i * cell_size };
        h_road.end = (Vector3){ map_size / 2, 0.0f, -map_size / 2 + i * cell_size };
        h_road.width = ROAD_WIDTH;
        h_road.roadColor = COLOR_ROAD_DARK;
        h_road.markingColor = { 225, 225, 255, 255 };
        h_road.hasGlowingMarkings = true;
        city.roads.push_back(h_road);

        // Routes verticales (X constant)
        RoadSegment v_road = {};
        v_road.start = (Vector3){ -map_size / 2 + i * cell_size, 0.0f, -map_size / 2 };
        v_road.end = (Vector3){ -map_size / 2 + i * cell_size, 0.0f, map_size / 2 };
        v_road.width = ROAD_WIDTH;
        v_road.roadColor = COLOR_ROAD_DARK;
        v_road.markingColor = COLOR_ROAD_NEON_MARKING;
        v_road.hasGlowingMarkings = true;
        city.roads.push_back(v_road);
    }
    
    // Génération des bâtiments
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            Building b = {};
            b.type = GetRandomFuturisticBuildingType();
            b.mainColor = GetFuturisticBuildingColor(b.type);
            b.accentColor = GetFuturisticAccentColor(b.type);
            
            // Position au centre de la cellule, avec un petit décalage aléatoire
            b.position.x = -map_size / 2 + i * cell_size + cell_size / 2 + GetRandomFloat(-5.0f, 5.0f);
            b.position.z = -map_size / 2 + j * cell_size + cell_size / 2 + GetRandomFloat(-5.0f, 5.0f);
            b.position.y = 0.0f; // La base est au sol

            // Taille du bâtiment (hauteur maximale réduite)
            b.size = (Vector3){ 
                GetRandomFloat(15.0f, 30.0f),  // Largeur
                GetRandomFloat(MIN_BUILDING_HEIGHT_GRID, MAX_BUILDING_HEIGHT_GRID), // Hauteur ajustée
                GetRandomFloat(15.0f, 30.0f)   // Profondeur
            };

            b.hasGlowingWindows = (rand() % 100) < 80;
            b.hasHelipad = (rand() % 100) < 15;
            b.glowIntensity = GetRandomFloat(0.6f, 1.0f);
            city.buildings.push_back(b);
        }
    }

    return city;
}

// Génération de la ville en anneaux (CIRCULAR_LAYOUT)
CityMap GenerateCircularLayout(int rings, float ring_spacing) {
    CityMap city;
    city.layout = CIRCULAR_LAYOUT;
    float current_radius = 0.0f;

    // Route centrale
    RoadSegment r_center = {};
    r_center.start = (Vector3){ -10.0f, 0.0f, 0.0f }; r_center.end = (Vector3){ 10.0f, 0.0f, 0.0f };
    r_center.width = ROAD_WIDTH;
    r_center.roadColor = COLOR_ROAD_DARK;
    r_center.markingColor = COLOR_ROAD_NEON_MARKING;
    r_center.hasGlowingMarkings = true;
    city.roads.push_back(r_center);

    for (int i = 0; i < rings; ++i) {
        current_radius += ring_spacing;
        
        // Simuler une route circulaire par 8 segments (pour simplifier le rendu raylib)
        for (int j = 0; j < 8; ++j) {
            RoadSegment r = {};
            float angle1 = (float)j * 45.0f * DEG2RAD;
            float angle2 = (float)(j + 1) * 45.0f * DEG2RAD;
            
            r.start = (Vector3){ current_radius * cos(angle1), 0.0f, current_radius * sin(angle1) };
            r.end = (Vector3){ current_radius * cos(angle2), 0.0f, current_radius * sin(angle2) };
            r.width = ROAD_WIDTH;
            r.roadColor = COLOR_ROAD_DARK;
            r.markingColor = COLOR_ROAD_NEON_MARKING;
            r.hasGlowingMarkings = true;
            city.roads.push_back(r);
        }

        // Ajout de bâtiments le long de l'anneau
        int buildings_per_ring = (int)(current_radius / 15.0f) * 2 + 10;
        for (int j = 0; j < buildings_per_ring; ++j) {
            Building b = {};
            b.type = GetRandomFuturisticBuildingType();
            b.mainColor = GetFuturisticBuildingColor(b.type);
            b.accentColor = GetFuturisticAccentColor(b.type);
            
            float angle = GetRandomFloat(0.0f, 360.0f) * DEG2RAD;
            float r = current_radius - ROAD_WIDTH / 2.0f - GetRandomFloat(5.0f, 15.0f);
            
            b.position.x = r * cos(angle);
            b.position.z = r * sin(angle);
            b.position.y = 0.0f;

            // Taille du bâtiment (hauteur maximale réduite)
            b.size = (Vector3){ 
                GetRandomFloat(12.0f, 28.0f),  // Largeur
                GetRandomFloat(MIN_BUILDING_HEIGHT_CIRCULAR, MAX_BUILDING_HEIGHT_CIRCULAR), // Hauteur ajustée
                GetRandomFloat(12.0f, 28.0f)   // Profondeur
            };

            b.hasGlowingWindows = (rand() % 100) < 85;
            b.hasHelipad = (rand() % 100) < 20;
            b.glowIntensity = GetRandomFloat(0.6f, 1.0f);
            city.buildings.push_back(b);
        }
    }

    return city;
}

// Génération de la ville organique (ORGANIC_LAYOUT)
CityMap GenerateOrganicLayout(int building_count, float area_size) {
    CityMap city;
    city.layout = ORGANIC_LAYOUT;

    // Routes basiques pour une zone centrale
    RoadSegment main_road_x = {};
    main_road_x.start = (Vector3){ -area_size/2, 0.0f, 0.0f };
    main_road_x.end = (Vector3){ area_size/2, 0.0f, 0.0f };
    main_road_x.width = ROAD_WIDTH;
    main_road_x.roadColor = COLOR_ROAD_DARK;
    main_road_x.markingColor = COLOR_ROAD_NEON_MARKING;
    main_road_x.hasGlowingMarkings = true;
    city.roads.push_back(main_road_x);

    RoadSegment main_road_z = {};
    main_road_z.start = (Vector3){ 0.0f, 0.0f, -area_size/2 };
    main_road_z.end = (Vector3){ 0.0f, 0.0f, area_size/2 };
    main_road_z.width = ROAD_WIDTH;
    main_road_z.roadColor = COLOR_ROAD_DARK;
    main_road_z.markingColor = COLOR_ROAD_NEON_MARKING;
    main_road_z.hasGlowingMarkings = true;
    city.roads.push_back(main_road_z);

    // Bâtiments organiques
    for (int i = 0; i < building_count; ++i) {
        Building b = {};
        b.type = GetRandomFuturisticBuildingType();
        b.mainColor = GetFuturisticBuildingColor(b.type);
        b.accentColor = GetFuturisticAccentColor(b.type);
        
        b.position.x = GetRandomFloat(-area_size/2, area_size/2);
        b.position.z = GetRandomFloat(-area_size/2, area_size/2);
        b.position.y = 0.0f;

        // Taille du bâtiment (hauteur maximale réduite)
        b.size = (Vector3){ 
            GetRandomFloat(10.0f, 25.0f),  // Largeur
            GetRandomFloat(MIN_BUILDING_HEIGHT_ORGANIC, MAX_BUILDING_HEIGHT_ORGANIC), // Hauteur ajustée
            GetRandomFloat(10.0f, 25.0f)   // Profondeur
        };

        b.hasGlowingWindows = (rand() % 100) < 70;
        b.hasHelipad = (rand() % 100) < 10;
        b.glowIntensity = GetRandomFloat(0.6f, 1.0f);
        city.buildings.push_back(b);
    }

    return city;
}

// Fonction principale pour générer la ville
CityMap GenerateFuturisticCity(int size, CityLayout layout) {
    // Initialisation de la graine aléatoire pour des résultats variés
    srand(time(NULL)); 
    
    switch (layout) {
        case GRID_LAYOUT:
            return GenerateGridLayout(size, 80.0f); // size est la taille de la grille (ex: 5x5)
        case CIRCULAR_LAYOUT:
            return GenerateCircularLayout(size, 80.0f); // size est le nombre d'anneaux
        case ORGANIC_LAYOUT:
            return GenerateOrganicLayout(size * 20, (float)size * 80.0f); // size est utilisé pour déterminer la densité
        default:
            return GenerateGridLayout(5, 80.0f); // Par défaut: grille 5x5
    }
}