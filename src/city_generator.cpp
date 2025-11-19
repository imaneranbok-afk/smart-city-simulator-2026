#include "city_generator.h"
#include "raymath.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>

float GetRandomFloat(float min, float max) {
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
        case COMMERCIAL_HUB: return COLOR_ACCENT_ORANGE;
        case OFFICE_COMPLEX: return COLOR_ACCENT_BLUE;
        case SKYSCRAPER: return COLOR_ACCENT_PURPLE;
        case ECO_BUILDING: return COLOR_ECO_NEON;
        case TRANSPORT_HUB: return COLOR_ACCENT_ORANGE;
        default: return COLOR_ACCENT_BLUE;
    }
}

CityMap GenerateFuturisticCity(int size, CityLayout layout) {
    switch (layout) {
        case GRID_LAYOUT: return GenerateGridLayout(size, 70.0f);
        case CIRCULAR_LAYOUT: return GenerateCircularLayout(size, 35.0f);
        case ORGANIC_LAYOUT: return GenerateOrganicLayout(size * 20, 200.0f);
        default: return GenerateGridLayout(size, 60.0f);
    }
}

CityMap GenerateGridLayout(int grid_size, float cell_size) {
    CityMap city;
    city.layout = GRID_LAYOUT;
    srand(time(NULL));

    // Routes
    for (int i = -grid_size; i <= grid_size; ++i) {
        RoadSegment h_road;
        h_road.start = (Vector3){ i * cell_size, 0.0f, -grid_size * cell_size };
        h_road.end = (Vector3){ i * cell_size, 0.0f, grid_size * cell_size };
        h_road.width = ROAD_WIDTH;
        h_road.roadColor = COLOR_ROAD_DARK;
        h_road.markingColor = COLOR_ROAD_NEON_MARKING;
        h_road.hasGlowingMarkings = true;
        h_road.hasCrosswalk = (abs(i) % 3 == 0);
        h_road.hasTrafficLight = (abs(i) % 4 == 0);
        city.roads.push_back(h_road);

        RoadSegment v_road;
        v_road.start = (Vector3){ -grid_size * cell_size, 0.0f, i * cell_size };
        v_road.end = (Vector3){ grid_size * cell_size, 0.0f, i * cell_size };
        v_road.width = ROAD_WIDTH;
        v_road.roadColor = COLOR_ROAD_DARK;
        v_road.markingColor = COLOR_ROAD_NEON_MARKING;
        v_road.hasGlowingMarkings = true;
        v_road.hasCrosswalk = (abs(i) % 3 == 0);
        v_road.hasTrafficLight = (abs(i) % 4 == 0);
        city.roads.push_back(v_road);
    }

    // Bâtiments
    for (int i = -grid_size; i < grid_size; ++i) {
        for (int j = -grid_size; j < grid_size; ++j) {
            if (rand() % 100 < 75) {
                Building b;
                b.type = GetRandomFuturisticBuildingType();
                b.mainColor = GetFuturisticBuildingColor(b.type);
                b.accentColor = GetFuturisticAccentColor(b.type);
                
                b.position.x = (i * cell_size) + GetRandomFloat(5.0f, cell_size - 5.0f);
                b.position.z = (j * cell_size) + GetRandomFloat(5.0f, cell_size - 5.0f);
                b.position.y = 0.0f;

                switch (b.type) {
                    case RESIDENTIAL_TOWER:
                        b.size = (Vector3){ GetRandomFloat(8.0f, 10.0f), GetRandomFloat(40.0f, 80.0f), GetRandomFloat(8.0f, 15.0f) };
                        break;
                    case COMMERCIAL_HUB:
                        b.size = (Vector3){ GetRandomFloat(10.0f, 25.0f), GetRandomFloat(25.0f, 45.0f), GetRandomFloat(20.0f, 35.0f) };
                        break;
                    case SKYSCRAPER:
                        b.size = (Vector3){ GetRandomFloat(25.0f, 40.0f), GetRandomFloat(100.0f, 200.0f), GetRandomFloat(25.0f, 40.0f) };
                        b.hasHelipad = true;
                        break;
                    default:
                        b.size = (Vector3){ GetRandomFloat(12.0f, 25.0f), GetRandomFloat(30.0f, 60.0f), GetRandomFloat(12.0f, 25.0f) };
                }

                b.hasGlowingWindows = true;
                b.glowIntensity = GetRandomFloat(0.5f, 1.0f);
                city.buildings.push_back(b);
            }
        }
    }

    GenerateHolographicTrafficLights(city);
    GenerateGlowingCrosswalks(city);
    return city;
}

CityMap GenerateCircularLayout(int rings, float ring_spacing) {
    CityMap city;
    city.layout = CIRCULAR_LAYOUT;
    srand(time(NULL));

    float center_x = 0.0f, center_z = 0.0f;

    // Routes radiales
    for (int angle = 0; angle < 360; angle += 45) {
        float rad_angle = angle * 3.14159f / 180.0f;
        float max_radius = rings * ring_spacing;
        
        RoadSegment radial_road;
        radial_road.start = (Vector3){ center_x, 0.0f, center_z };
        radial_road.end = (Vector3){ 
            center_x + max_radius * sin(rad_angle), 
            0.0f, 
            center_z + max_radius * cos(rad_angle) 
        };
        radial_road.width = ROAD_WIDTH;
        radial_road.roadColor = COLOR_ROAD_DARK;
        radial_road.markingColor = COLOR_ROAD_NEON_MARKING;
        radial_road.hasCrosswalk = (angle % 90 == 0);
        city.roads.push_back(radial_road);
    }

    // Bâtiments en cercles
    for (int ring = 1; ring <= rings; ++ring) {
        float radius = ring * ring_spacing;
        int buildings_in_ring = 8 * ring;
        
        for (int i = 0; i < buildings_in_ring; ++i) {
            float angle = (2 * 3.14159f * i) / buildings_in_ring;
            
            Building b;
            b.type = GetRandomFuturisticBuildingType();
            b.mainColor = GetFuturisticBuildingColor(b.type);
            b.accentColor = GetFuturisticAccentColor(b.type);
            
            b.position.x = center_x + radius * sin(angle);
            b.position.z = center_z + radius * cos(angle);
            b.position.y = 0.0f;

            b.size = (Vector3){ 
                GetRandomFloat(8.0f, 20.0f), 
                GetRandomFloat(30.0f, 80.0f), 
                GetRandomFloat(8.0f, 15.0f) 
            };

            b.hasGlowingWindows = true;
            b.glowIntensity = GetRandomFloat(0.6f, 1.0f);
            city.buildings.push_back(b);
        }
    }

    return city;
}

CityMap GenerateOrganicLayout(int building_count, float area_size) {
    CityMap city;
    city.layout = ORGANIC_LAYOUT;
    srand(time(NULL));

    // Routes basiques
    RoadSegment main_road;
    main_road.start = (Vector3){ -area_size/2, 0.0f, 0.0f };
    main_road.end = (Vector3){ area_size/2, 0.0f, 0.0f };
    main_road.width = ROAD_WIDTH;
    main_road.roadColor = COLOR_ROAD_DARK;
    main_road.markingColor = COLOR_ROAD_NEON_MARKING;
    city.roads.push_back(main_road);

    // Bâtiments organiques
    for (int i = 0; i < building_count; ++i) {
        Building b;
        b.type = GetRandomFuturisticBuildingType();
        b.mainColor = GetFuturisticBuildingColor(b.type);
        b.accentColor = GetFuturisticAccentColor(b.type);
        
        b.position.x = GetRandomFloat(-area_size/2, area_size/2);
        b.position.z = GetRandomFloat(-area_size/2, area_size/2);
        b.position.y = 0.0f;

        b.size = (Vector3){ 
            GetRandomFloat(10.0f, 25.0f), 
            GetRandomFloat(20.0f, 70.0f), 
            GetRandomFloat(10.0f, 25.0f) 
        };

        b.hasGlowingWindows = true;
        b.glowIntensity = GetRandomFloat(0.4f, 0.9f);
        city.buildings.push_back(b);
    }

    return city;
}

void GenerateHolographicTrafficLights(CityMap& city) {
    // Implémentation simplifiée
}

void GenerateGlowingCrosswalks(CityMap& city) {
    // Implémentation simplifiée
}