#include "raylib.h"
#include "raymath.h"
#include "city_data.h"
#include "city_generator.h"
#include "emergency_system.h"
#include "camera_controller.h"
#include <cmath>
#include <vector>
#include <string>

// Variables globales externes
extern CustomCameraMode currentCameraMode;
extern Vehicle emergencyVehicle;
extern Emergency currentEmergency;

// Constantes
const int screenWidth = 1200;
const int screenHeight = 900;

// Fonction pour dessiner un segment de route
void DrawRoadSegment(const RoadSegment& road) {
    Vector3 roadCenter = { 
        (road.start.x + road.end.x) * 0.5f,
        0.0f,
        (road.start.z + road.end.z) * 0.5f
    };
    
    float dx = fabsf(road.end.x - road.start.x);
    float dz = fabsf(road.end.z - road.start.z);
    float length = Vector3Distance(road.start, road.end);

    if (dx > dz) {
        DrawCube(roadCenter, length, 0.1f, road.width, road.roadColor);
    } else {
        DrawCube(roadCenter, road.width, 0.1f, length, road.roadColor);
    }
}

// Fonction pour dessiner un bâtiment
void DrawBuilding(const Building& building) {
    Vector3 buildingCenter = { 
        building.position.x, 
        building.size.y * 0.5f, 
        building.position.z 
    };
    
    DrawCube(buildingCenter, building.size.x, building.size.y, building.size.z, building.mainColor);
    DrawCubeWires(buildingCenter, building.size.x, building.size.y, building.size.z, BLACK);
}

// Fonction pour dessiner la ville complète
void DrawCity(const CityMap& city) {
    // Sol
    float groundSize = 1000.0f;
    DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ groundSize, groundSize }, (Color){ 50, 180, 80, 255 });

    // Routes
    for (size_t i = 0; i < city.roads.size(); i++) {
        DrawRoadSegment(city.roads[i]);
    }

    // Bâtiments
    for (size_t i = 0; i < city.buildings.size(); i++) {
        DrawBuilding(city.buildings[i]);
    }
}

// Fonction pour dessiner la scène complète
void DrawScene(Camera3D camera, const CityMap& city) {
    BeginDrawing();
    ClearBackground((Color){ 10, 10, 40, 255 });

    BeginMode3D(camera);
        DrawCity(city);
        DrawEmergencyEffects(city);
        DrawGrid(100, 10.0f);
    EndMode3D();

    // UI
    DrawRectangle(0, 0, screenWidth, 100, Fade(BLACK, 0.7f));
    
    const char* modeName = "";
    switch (currentCameraMode) {
        case CUSTOM_CAMERA_MODE_FREE: modeName = "LIBRE"; break;
        case CUSTOM_CAMERA_MODE_FOLLOW: modeName = "SUIVEUR"; break;
        case CUSTOM_CAMERA_MODE_STRATEGIC: modeName = "STRATEGIQUE"; break;
        default: modeName = "INCONNU";
    }
    
    DrawText(TextFormat("Mode Camera: %s", modeName), 20, 15, 22, GREEN);
    DrawText(TextFormat("Position: (%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z), 20, 40, 18, WHITE);
    
    // Statut urgence
    Color emergencyColor = currentEmergency.is_active ? RED : GREEN;
    const char* emergencyStatus = currentEmergency.is_active ? "ACTIVE" : "INACTIVE";
    DrawText(TextFormat("URGENCE: %s", emergencyStatus), 20, 65, 24, emergencyColor);
    DrawText("ESPACE: Urgence | F1/F2/F3: Ville", 20, screenHeight - 30, 20, WHITE);

    DrawFPS(screenWidth - 100, 15);
    EndDrawing();
}

// Fonction principale
int main(void) {
    // Initialisation
    InitWindow(screenWidth, screenHeight, "Smart City 3D Simulator - Ville Futuriste");
    SetTargetFPS(60);

    // Génération de la ville
    CityMap city = GenerateFuturisticCity(8, GRID_LAYOUT);

    // Initialisation systèmes
    Camera3D camera = createBaseCamera();
    InitEmergencySystem(city);

    // Boucle principale
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Mise à jour caméra
        UpdateCameraMode(&camera, emergencyVehicle.position);

        // Mise à jour systèmes
        UpdateEmergencySystem(deltaTime);

        // Gestion urgence
        if (IsKeyPressed(KEY_SPACE)) {
            if (currentEmergency.is_active) {
                StopEmergency();
            } else {
                StartEmergency(city);
            }
        }

        // Changement de layout
        if (IsKeyPressed(KEY_F1)) {
            city = GenerateFuturisticCity(8, GRID_LAYOUT);
            InitEmergencySystem(city);
        }
        if (IsKeyPressed(KEY_F2)) {
            city = GenerateFuturisticCity(6, CIRCULAR_LAYOUT);
            InitEmergencySystem(city);
        }
        if (IsKeyPressed(KEY_F3)) {
            city = GenerateFuturisticCity(200, ORGANIC_LAYOUT);
            InitEmergencySystem(city);
        }

        // Dessin
        DrawScene(camera, city);
    }

    CloseWindow();
    return 0;
}