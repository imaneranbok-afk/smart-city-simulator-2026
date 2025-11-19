#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <vector>
#include <string>

// === STRUCTURES SIMPLIFIEES ===
// SUPPRIMER ces enum - ils existent déjà dans Raylib !
/*
typedef enum {
    CAMERA_FREE,
    CAMERA_FIRST_PERSON, 
    CAMERA_THIRD_PERSON
} CameraMode;
*/

// Utiliser directement les enum de Raylib :
// - CameraMode existe déjà dans raylib.h
// - Pas besoin de les redéfinir

typedef enum {
    GRID_LAYOUT,
    CIRCULAR_LAYOUT, 
    ORGANIC_LAYOUT
} CityLayout;

typedef enum {
    RESIDENTIAL_TOWER,
    COMMERCIAL_HUB,
    OFFICE_COMPLEX
} BuildingType;

struct Building {
    Vector3 position;
    Vector3 size;
    Color mainColor;
    Color accentColor;
    BuildingType type;
    bool hasGlowingWindows;
};

struct RoadSegment {
    Vector3 start;
    Vector3 end;
    float width;
    Color roadColor;
    Color markingColor;
};

struct TrafficLight {
    Vector3 position;
    bool isRed;
    bool isGreen;
};

struct Crosswalk {
    Vector3 position;
    float width;
    float length;
    bool isActive;
};

struct CityMap {
    std::vector<Building> buildings;
    std::vector<RoadSegment> roads;
    std::vector<TrafficLight> trafficLights;
    std::vector<Crosswalk> crosswalks;
};

struct EmergencyVehicle {
    Vector3 position;
    Vector3 target;
    bool isActive;
};

struct EmergencyEvent {
    Vector3 position;
    bool isActive;
};

// === VARIABLES GLOBALES ===
// Utiliser le CameraMode de Raylib directement
::CameraMode currentCameraMode = ::CAMERA_FREE; // :: pour être explicite
EmergencyVehicle emergencyVehicle = {0};
EmergencyEvent currentEmergency = {0};

// === CONSTANTES ===
const int screenWidth = 1200;
const int screenHeight = 900;
const int CITY_GRID_SIZE = 8;
const float CITY_CELL_SIZE = 40.0f;

// === FONCTIONS DE GENERATION SIMPLIFIEES ===
float GetRandomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
}

CityMap GenerateGridLayout(int grid_size, float cell_size) {
    CityMap city;
    
    // Routes
    for (int i = -grid_size; i <= grid_size; ++i) {
        RoadSegment h_road;
        h_road.start = (Vector3){ i * cell_size, 0.0f, -grid_size * cell_size };
        h_road.end = (Vector3){ i * cell_size, 0.0f, grid_size * cell_size };
        h_road.width = 10.0f;
        h_road.roadColor = (Color){ 30, 30, 40, 255 };
        h_road.markingColor = (Color){ 0, 255, 255, 255 };
        city.roads.push_back(h_road);

        RoadSegment v_road;
        v_road.start = (Vector3){ -grid_size * cell_size, 0.0f, i * cell_size };
        v_road.end = (Vector3){ grid_size * cell_size, 0.0f, i * cell_size };
        v_road.width = 10.0f;
        v_road.roadColor = (Color){ 30, 30, 40, 255 };
        v_road.markingColor = (Color){ 0, 255, 255, 255 };
        city.roads.push_back(v_road);
    }

    // Bâtiments
    for (int i = -grid_size; i < grid_size; ++i) {
        for (int j = -grid_size; j < grid_size; ++j) {
            if (rand() % 100 < 70) {
                Building b;
                b.position = (Vector3){ 
                    i * cell_size + GetRandomFloat(5.0f, cell_size - 5.0f),
                    0.0f,
                    j * cell_size + GetRandomFloat(5.0f, cell_size - 5.0f)
                };
                b.size = (Vector3){ 
                    GetRandomFloat(8.0f, 20.0f),
                    GetRandomFloat(20.0f, 60.0f), 
                    GetRandomFloat(8.0f, 20.0f)
                };
                b.mainColor = (Color){ 
                    static_cast<unsigned char>(100 + rand() % 100),
                    static_cast<unsigned char>(100 + rand() % 100),
                    static_cast<unsigned char>(100 + rand() % 100),
                    255
                };
                b.hasGlowingWindows = true;
                city.buildings.push_back(b);
            }
        }
    }

    return city;
}

// === FONCTIONS CAMERA SIMPLIFIEES ===
Camera3D createBaseCamera() {
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 100.0f, 120.0f, 100.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return camera;
}

void UpdateCameraMode(Camera3D* camera, const Vector3& target) {
    // Simulation basique de mouvement de caméra
    if (IsKeyDown(KEY_W)) camera->position.z -= 1.0f;
    if (IsKeyDown(KEY_S)) camera->position.z += 1.0f;
    if (IsKeyDown(KEY_A)) camera->position.x -= 1.0f;
    if (IsKeyDown(KEY_D)) camera->position.x += 1.0f;
    if (IsKeyDown(KEY_Q)) camera->position.y -= 1.0f;
    if (IsKeyDown(KEY_E)) camera->position.y += 1.0f;

    // Changement de mode - utiliser les constantes de Raylib
    if (IsKeyPressed(KEY_ONE)) currentCameraMode = ::CAMERA_FREE;
    if (IsKeyPressed(KEY_TWO)) currentCameraMode = ::CAMERA_FIRST_PERSON;
    if (IsKeyPressed(KEY_THREE)) currentCameraMode = ::CAMERA_THIRD_PERSON;
}

// === SYSTEME D'URGENCE SIMPLIFIE ===
void InitEmergencySystem(const CityMap& city) {
    emergencyVehicle.position = (Vector3){ 0.0f, 2.0f, 0.0f };
    emergencyVehicle.isActive = false;
    currentEmergency.isActive = false;
}

void UpdateEmergencySystem(float deltaTime) {
    if (currentEmergency.isActive) {
        // Simulation de mouvement du véhicule d'urgence
        emergencyVehicle.position.x += sin(GetTime()) * 5.0f * deltaTime;
        emergencyVehicle.position.z += cos(GetTime()) * 5.0f * deltaTime;
    }
}

void DrawEmergencyEffects(const CityMap& city) {
    if (currentEmergency.isActive) {
        // Effet de lumière d'urgence (sirène)
        float pulse = 0.5f + 0.5f * sin(GetTime() * 10.0f);
        DrawSphere(emergencyVehicle.position, 3.0f, Fade(RED, pulse));
        DrawSphere(emergencyVehicle.position, 5.0f, Fade(BLUE, 0.3f * pulse));
    }
}

void StartEmergency(const CityMap& city) {
    currentEmergency.isActive = true;
    emergencyVehicle.isActive = true;
    // Position aléatoire pour l'urgence
    currentEmergency.position = (Vector3){
        GetRandomFloat(-50.0f, 50.0f),
        0.0f,
        GetRandomFloat(-50.0f, 50.0f)
    };
}

void StopEmergency() {
    currentEmergency.isActive = false;
    emergencyVehicle.isActive = false;
}

// === FONCTIONS DE DESSIN ===
void DrawRoadSegment(const RoadSegment& road) {
    Vector3 roadCenter = Vector3Scale(Vector3Add(road.start, road.end), 0.5f);
    float length = Vector3Length(Vector3Subtract(road.end, road.start));
    
    float dx = fabsf(road.end.x - road.start.x);
    float dz = fabsf(road.end.z - road.start.z);

    if (dx > dz) {
        DrawCube(roadCenter, length, 0.1f, road.width, road.roadColor);
    } else {
        DrawCube(roadCenter, road.width, 0.1f, length, road.roadColor);
    }
}

void DrawBuilding(const Building& building) {
    Vector3 buildingCenter = { 
        building.position.x, 
        building.size.y / 2.0f, 
        building.position.z 
    };
    
    DrawCube(buildingCenter, building.size.x, building.size.y, building.size.z, building.mainColor);
    DrawCubeWires(buildingCenter, building.size.x, building.size.y, building.size.z, BLACK);
}

void DrawCity(const CityMap& city) {
    // Sol
    float groundSize = CITY_CELL_SIZE * CITY_GRID_SIZE * 2.0f;
    DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ groundSize, groundSize }, (Color){ 50, 180, 80, 255 });

    // Routes
    for (const auto& road : city.roads) {
        DrawRoadSegment(road);
    }

    // Bâtiments
    for (const auto& building : city.buildings) {
        DrawBuilding(building);
    }
}

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
        case ::CAMERA_FREE: modeName = "LIBRE"; break;
        case ::CAMERA_FIRST_PERSON: modeName = "PREMIERE PERSONNE"; break;
        case ::CAMERA_THIRD_PERSON: modeName = "SUIVEUR"; break;
        default: modeName = "INCONNU";
    }
    
    DrawText(TextFormat("Mode Camera: %s", modeName), 20, 15, 22, GREEN);
    DrawText(TextFormat("Position: (%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z), 20, 40, 18, WHITE);
    
    // Statut urgence
    Color emergencyColor = currentEmergency.isActive ? RED : GREEN;
    const char* emergencyStatus = currentEmergency.isActive ? "ACTIVE" : "INACTIVE";
    DrawText(TextFormat("URGENCE: %s", emergencyStatus), 20, 65, 24, emergencyColor);
    DrawText("ESPACE: activer/desactiver l'urgence", 20, screenHeight - 30, 20, WHITE);

    DrawFPS(screenWidth - 100, 15);
    EndDrawing();
}

// === FONCTION MAIN ===
int main(void)
{
    // Initialisation
    InitWindow(screenWidth, screenHeight, "Smart City 3D Simulator");
    SetTargetFPS(60);

    // Génération de la ville
    CityMap city = GenerateGridLayout(CITY_GRID_SIZE, CITY_CELL_SIZE);

    // Initialisation systèmes
    Camera3D camera = createBaseCamera();
    InitEmergencySystem(city);

    // Boucle principale
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Mise à jour caméra
        UpdateCameraMode(&camera, emergencyVehicle.position);

        // Mise à jour systèmes
        UpdateEmergencySystem(deltaTime);

        // Gestion urgence
        if (IsKeyPressed(KEY_SPACE)) {
            if (currentEmergency.isActive) {
                StopEmergency();
            } else {
                StartEmergency(city);
            }
        }

        // Dessin
        DrawScene(camera, city);
    }

    CloseWindow();
    return 0;
}