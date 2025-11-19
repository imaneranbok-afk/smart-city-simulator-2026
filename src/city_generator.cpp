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
        case GRID_LAYOUT:
            return GenerateGridLayout(size, 40.0f);
        case CIRCULAR_LAYOUT:
            return GenerateCircularLayout(size, 35.0f);
        case ORGANIC_LAYOUT:
            return GenerateOrganicLayout(size * 20, 200.0f);
        default:
            return GenerateGridLayout(size, 40.0f);
    }
}

// GRILLE MODERNE
CityMap GenerateGridLayout(int grid_size, float cell_size) {
    CityMap city;
    city.layout = GRID_LAYOUT;
    srand(time(NULL));

    // Routes en grille avec marquages néon
    for (int i = -grid_size; i <= grid_size; ++i) {
        // Routes horizontales
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

        // Routes verticales
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

    // Bâtiments futuristes en grille
    for (int i = -grid_size; i < grid_size; ++i) {
        for (int j = -grid_size; j < grid_size; ++j) {
            if (rand() % 100 < 75) { // Densité élevée
                Building b;
                b.type = GetRandomFuturisticBuildingType();
                b.mainColor = GetFuturisticBuildingColor(b.type);
                b.accentColor = GetFuturisticAccentColor(b.type);
                
                // Position dans la cellule
                b.position.x = (i * cell_size) + GetRandomFloat(5.0f, cell_size - 5.0f);
                b.position.z = (j * cell_size) + GetRandomFloat(5.0f, cell_size - 5.0f);
                b.position.y = 0.0f;

                // Tailles selon le type
                switch (b.type) {
                    case RESIDENTIAL_TOWER:
                        b.size = (Vector3){ GetRandomFloat(8.0f, 15.0f), GetRandomFloat(40.0f, 80.0f), GetRandomFloat(8.0f, 15.0f) };
                        break;
                    case COMMERCIAL_HUB:
                        b.size = (Vector3){ GetRandomFloat(20.0f, 35.0f), GetRandomFloat(25.0f, 45.0f), GetRandomFloat(20.0f, 35.0f) };
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

// ORGANISATION CIRCULAIRE
CityMap GenerateCircularLayout(int rings, float ring_spacing) {
    CityMap city;
    city.layout = CIRCULAR_LAYOUT;
    srand(time(NULL));

    float center_x = 0.0f;
    float center_z = 0.0f;

    // Routes circulaires
    for (int ring = 1; ring <= rings; ++ring) {
        float radius = ring * ring_spacing;
        
        // Route circulaire
        RoadSegment circular_road;
        circular_road.width = ROAD_WIDTH + 2.0f;
        circular_road.roadColor = COLOR_ROAD_DARK;
        circular_road.markingColor = COLOR_ROAD_NEON_MARKING;
        circular_road.hasGlowingMarkings = true;
        circular_road.hasTrafficLight = (ring % 2 == 0);
        city.roads.push_back(circular_road);

        // Routes radiales (tous les 45 degrés)
        for (int angle = 0; angle < 360; angle += 45) {
            float rad_angle = angle * 3.14159f / 180.0f;
            
            RoadSegment radial_road;
            radial_road.start = (Vector3){ center_x, 0.0f, center_z };
            radial_road.end = (Vector3){ 
                center_x + (radius + ring_spacing) * sin(rad_angle), 
                0.0f, 
                center_z + (radius + ring_spacing) * cos(rad_angle) 
            };
            radial_road.width = ROAD_WIDTH;
            radial_road.roadColor = COLOR_ROAD_DARK;
            radial_road.markingColor = COLOR_ROAD_NEON_MARKING;
            radial_road.hasCrosswalk = (angle % 90 == 0);
            city.roads.push_back(radial_road);
        }
    }

    // Bâtiments en cercles concentriques
    for (int ring = 1; ring <= rings; ++ring) {
        float radius = ring * ring_spacing;
        int buildings_in_ring = 8 * ring; // Plus de bâtiments dans les rings externes
        
        for (int i = 0; i < buildings_in_ring; ++i) {
            float angle = (2 * 3.14159f * i) / buildings_in_ring;
            float variation = GetRandomFloat(-ring_spacing * 0.3f, ring_spacing * 0.3f);
            
            Building b;
            b.type = GetRandomFuturisticBuildingType();
            b.mainColor = GetFuturisticBuildingColor(b.type);
            b.accentColor = GetFuturisticAccentColor(b.type);
            
            b.position.x = center_x + (radius + variation) * sin(angle);
            b.position.z = center_z + (radius + variation) * cos(angle);
            b.position.y = 0.0f;

            // Orientation radiale
            b.size = (Vector3){ 
                GetRandomFloat(8.0f, 20.0f), 
                GetRandomFloat(30.0f, 80.0f), 
                GetRandomFloat(8.0f, 15.0f) 
            };

            b.hasGlowingWindows = true;
            b.glowIntensity = GetRandomFloat(0.6f, 1.0f);
            b.hasHelipad = (b.type == SKYSCRAPER && ring == rings);
            
            city.buildings.push_back(b);
        }
    }

    GenerateHolographicTrafficLights(city);
    return city;
}

// ORGANISATION ORGANIQUE
CityMap GenerateOrganicLayout(int building_count, float area_size) {
    CityMap city;
    city.layout = ORGANIC_LAYOUT;
    srand(time(NULL));

    // Routes organiques (courbes)
    std::vector<Vector3> controlPoints = {
        (Vector3){ -area_size * 0.8f, 0.0f, -area_size * 0.8f },
        (Vector3){ -area_size * 0.3f, 0.0f, area_size * 0.2f },
        (Vector3){ area_size * 0.2f, 0.0f, -area_size * 0.1f },
        (Vector3){ area_size * 0.7f, 0.0f, area_size * 0.6f }
    };

    // Génération de routes courbes
    for (size_t i = 0; i < controlPoints.size() - 1; ++i) {
        RoadSegment organic_road;
        organic_road.start = controlPoints[i];
        organic_road.end = controlPoints[i + 1];
        organic_road.width = ROAD_WIDTH;
        organic_road.roadColor = COLOR_ROAD_DARK;
        organic_road.markingColor = COLOR_ROAD_NEON_MARKING;
        organic_road.hasGlowingMarkings = true;
        organic_road.hasCrosswalk = (i % 2 == 0);
        city.roads.push_back(organic_road);
    }

    // Bâtiments organiques (placement aléatoire avec clustering)
    for (int i = 0; i < building_count; ++i) {
        Building b;
        b.type = GetRandomFuturisticBuildingType();
        b.mainColor = GetFuturisticBuildingColor(b.type);
        b.accentColor = GetFuturisticAccentColor(b.type);
        
        // Placement organique (groupes naturels)
        float cluster_center_x = GetRandomFloat(-area_size * 0.5f, area_size * 0.5f);
        float cluster_center_z = GetRandomFloat(-area_size * 0.5f, area_size * 0.5f);
        
        b.position.x = cluster_center_x + GetRandomFloat(-area_size * 0.1f, area_size * 0.1f);
        b.position.z = cluster_center_z + GetRandomFloat(-area_size * 0.1f, area_size * 0.1f);
        b.position.y = 0.0f;

        // Formes organiques variées
        b.size = (Vector3){ 
            GetRandomFloat(10.0f, 25.0f), 
            GetRandomFloat(20.0f, 70.0f), 
            GetRandomFloat(10.0f, 25.0f) 
        };

        b.hasGlowingWindows = true;
        b.glowIntensity = GetRandomFloat(0.4f, 0.9f);
        b.hasHelipad = (b.type == SKYSCRAPER && rand() % 10 == 0);
        
        city.buildings.push_back(b);
    }

    GenerateGlowingCrosswalks(city);
    return city;
}

void GenerateHolographicTrafficLights(CityMap& city) {
    for (auto& road : city.roads) {
        if (road.hasTrafficLight) {
            TrafficLight light;
            light.position = (Vector3){ 
                road.start.x + (road.end.x - road.start.x) * 0.5f,
                8.0f, // Hauteur
                road.start.z + (road.end.z - road.start.z) * 0.5f
            };
            light.isRed = (rand() % 2 == 0);
            light.isYellow = false;
            light.isGreen = !light.isRed;
            light.timer = GetRandomFloat(5.0f, 15.0f);
            light.glowEffect = 1.0f;
            
            city.trafficLights.push_back(light);
            road.trafficLightPos = light.position;
        }
    }
}

void GenerateGlowingCrosswalks(CityMap& city) {
    for (auto& road : city.roads) {
        if (road.hasCrosswalk) {
            Crosswalk crosswalk;
            crosswalk.position = (Vector3){ 
                road.start.x + (road.end.x - road.start.x) * 0.5f,
                0.02f, // Légèrement au-dessus de la route
                road.start.z + (road.end.z - road.start.z) * 0.5f
            };
            crosswalk.width = road.width * 0.8f;
            crosswalk.length = 4.0f;
            crosswalk.isActive = true;
            crosswalk.pulseIntensity = 0.0f;
            
            city.crosswalks.push_back(crosswalk);
        }
    }
}