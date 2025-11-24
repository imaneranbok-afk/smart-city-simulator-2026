#include "raylib.h"
#include "raymath.h"
#include <cstdlib>
#include <ctime>
#include "city_data.h"
#include "city_generator.h"
#include "camera_controller.h"
#include "emergency_system.h" 
#include "city_renderer.h" 
#include "emergency_data.h" 

// Déclaration des variables globales
// Elles sont DEFINIES dans les fichiers .cpp respectifs
extern Vehicle emergencyVehicle; 
extern Emergency currentEmergency;
extern CustomCameraMode currentCameraMode; 

// Fonctions externes :
extern Camera3D createBaseCamera();
extern void InitEmergencySystem(const CityMap& city);
extern void UpdateCameraMode(Camera3D* camera, const Vector3& followTarget);
extern void UpdateEmergencySystem(float deltaTime);
extern void StartEmergency(CityMap& city);
extern void StopEmergency();
extern void DrawCityMap(const CityMap& city);


int main(void) {
    // --- Configuration et Initialisation ---
    const int screenWidth = 1400; 
    const int screenHeight = 900;

    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    InitWindow(screenWidth, screenHeight, "City Simulator Futuriste - Rendu Complet");

    srand(time(NULL));

    // 1. Génération de la Cité: 6x6 blocs de taille moyenne
    // La fonction GenerateFuturisticCity utilise 6 comme taille de grille et CELL_SIZE_MEDIUM (100.0f)
    CityMap city = GenerateFuturisticCity(6, GRID_LAYOUT); 

    // 2. Initialisation de la Caméra
    Camera3D camera = createBaseCamera(); 
    currentCameraMode = CUSTOM_CAMERA_MODE_STRATEGIC;
    // Position de départ centrée au-dessus de la carte
    camera.position = (Vector3){ 0.0f, 300.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    
    // 3. Initialisation du Système d'Urgence
    InitEmergencySystem(city); 

    SetTargetFPS(60); 

    // --- Boucle Principale du Jeu ---
    while (!WindowShouldClose()) {
        
        // --- Mise à Jour (LOGIQUE) ---
        float deltaTime = GetFrameTime();
        Vector3 followTarget = emergencyVehicle.position; 
        
        UpdateCameraMode(&camera, followTarget); 
        UpdateEmergencySystem(deltaTime); 

        // Contrôles
        if (IsKeyPressed(KEY_SPACE) && !currentEmergency.is_active) {
            StartEmergency(city);
        }
        else if (IsKeyPressed(KEY_ENTER) && currentEmergency.is_active) {
            StopEmergency();
        }

        // --- Rendu (DESSIN) ---
        BeginDrawing();

            // ClearBackground(GREEN); // Fond noir complet

            BeginMode3D(camera);
                
                // Appel à la fonction définie dans city_renderer.cpp
                DrawCityMap(city); 
                DrawGrid(200, 10.0f); // Grille de référence

            EndMode3D();

            // Dessin de l'interface utilisateur
            DrawFPS(10, 10);
            DrawText("Controles Camera: [1] Libre, [2] Suivre Vehicule, [3] Strategique (Z/Q/S/D pour se deplacer)", 10, 30, 20, RAYWHITE);
            DrawText("Controles Urgence: [ESPACE] Demarrer, [ENTER] Arreter", 10, 50, 20, RAYWHITE);
            DrawText(TextFormat("Mode Actuel: %d (3 = Strategique)", currentCameraMode), 10, 70, 20, RAYWHITE);
            
            // Affichage de l'état de l'urgence
            if (currentEmergency.is_active) {
                DrawText("URGENCE ACTIVE", screenWidth - 200, 10, 20, YELLOW);
            }

        EndDrawing();
    }

    // --- Fermeture ---
    CloseWindow();
    return 0;
}