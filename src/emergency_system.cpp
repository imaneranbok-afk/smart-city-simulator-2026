#include "emergency_system.h"
#include "raymath.h"
#include <cmath>

// Variables globales définies dans emergency_data.h
Vehicle emergencyVehicle = { 0 };
Emergency currentEmergency = { 0 };

// Initialisation simple du véhicule et de l'urgence
void InitEmergencySystem(const CityMap& city) {
    // Initialisation du véhicule d'urgence
    emergencyVehicle.position = (Vector3){ 0.0f, 1.0f, 0.0f };
    emergencyVehicle.direction = (Vector3){ 1.0f, 0.0f, 0.0f };
    emergencyVehicle.speed = 0.0f;
    emergencyVehicle.color = RED;
    emergencyVehicle.is_priority = false;
    emergencyVehicle.id = 1;

    // Initialisation de l'urgence
    currentEmergency.is_active = false;
    currentEmergency.intensity = 0.0f;
    currentEmergency.target_building_index = -1;
}

// Fonction pour démarrer une urgence
void StartEmergency(CityMap& city) {
    if (city.buildings.empty()) return;

    // Sélection aléatoire d'un bâtiment important comme cible
    int targetIndex = -1;
    for (size_t i = 0; i < city.buildings.size(); ++i) {
        if (city.buildings[i].name == "Important") {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        // Si aucun bâtiment important n'est trouvé, prendre le premier
        targetIndex = 0;
    }

    currentEmergency.location = city.buildings[targetIndex].position;
    currentEmergency.is_active = true;
    currentEmergency.intensity = 1.0f;
    currentEmergency.target_building_index = targetIndex;

    emergencyVehicle.is_priority = true;
    emergencyVehicle.speed = 5.0f; // Démarrage du véhicule
}

// Fonction pour arrêter l'urgence
void StopEmergency() {
    currentEmergency.is_active = false;
    currentEmergency.intensity = 0.0f;
    currentEmergency.target_building_index = -1;
    emergencyVehicle.is_priority = false;
    emergencyVehicle.speed = 0.0f;
}

// Mise à jour de la simulation (mouvement simple du véhicule)
void UpdateEmergencySystem(float deltaTime) {
    if (emergencyVehicle.is_priority) {
        // Mouvement simple vers la cible (pour l'instant, sans gestion de route)
        Vector3 target = currentEmergency.location;
        Vector3 toTarget = Vector3Subtract(target, emergencyVehicle.position);
        float distance = Vector3Length(toTarget);

        if (distance > 1.0f) {
            emergencyVehicle.direction = Vector3Normalize(toTarget);
            Vector3 velocity = Vector3Scale(emergencyVehicle.direction, emergencyVehicle.speed * deltaTime);
            emergencyVehicle.position = Vector3Add(emergencyVehicle.position, velocity);
        } else {
            // Arrivé à destination
            emergencyVehicle.speed = 0.0f;
        }
    }
}

// Dessin des effets visuels
void DrawEmergencyEffects(const CityMap& city) {
    // 1. Surbrillance du bâtiment cible
    if (currentEmergency.is_active && currentEmergency.target_building_index != -1) {
        const Building& targetBuilding = city.buildings[currentEmergency.target_building_index];
        Vector3 center = { targetBuilding.position.x, targetBuilding.position.y + targetBuilding.size.y / 2.0f, targetBuilding.position.z };

        // Effet de pulsation (glow)
        float pulse = (sin(GetTime() * 5.0f) + 1.0f) / 2.0f; // 0.0 à 1.0
        Color highlightColor = ColorAlpha(COLOR_HIGHLIGHT_BUILDING, 0.5f + pulse * 0.5f);

        // Dessin d'un cube plus grand et transparent autour du bâtiment
        DrawCube(center, targetBuilding.size.x * 1.1f, targetBuilding.size.y * 1.1f, targetBuilding.size.z * 1.1f, highlightColor);
    }

    // 2. Dessin du véhicule et des sirènes
    if (emergencyVehicle.is_priority) {
        // Dessin du véhicule (cube simple)
        Vector3 vehicleSize = { 2.0f, 1.5f, 4.0f };
        Vector3 vehicleCenter = { emergencyVehicle.position.x, emergencyVehicle.position.y + vehicleSize.y / 2.0f, emergencyVehicle.position.z };
        DrawCube(vehicleCenter, vehicleSize.x, vehicleSize.y, vehicleSize.z, emergencyVehicle.color);
        DrawCubeWires(vehicleCenter, vehicleSize.x, vehicleSize.y, vehicleSize.z, BLACK);

        // Effet de sirène (lumière projetée)
        float sirenTime = GetTime() * 10.0f;
        Color sirenColor = (fmod(sirenTime, 2.0f) < 1.0f) ? COLOR_SIREN_RED : COLOR_SIREN_BLUE;

        // Dessin d'une sphère lumineuse au-dessus du véhicule
        Vector3 lightPos = Vector3Add(vehicleCenter, (Vector3){ 0.0f, vehicleSize.y / 2.0f + 0.5f, 0.0f });
        DrawSphere(lightPos, 0.3f, sirenColor);

        // Simulation de la lumière projetée (cône ou sphère transparente)
        Color lightEffectColor = ColorAlpha(sirenColor, 0.1f);
        DrawSphere(lightPos, 10.0f, lightEffectColor);

        // 3. Traînée de mouvement (simplifiée)
        // Pour une traînée réelle, il faudrait stocker les positions précédentes.
        // Ici, on simule une traînée par un simple plan derrière le véhicule.
        Vector3 trailStart = Vector3Subtract(emergencyVehicle.position, Vector3Scale(emergencyVehicle.direction, 5.0f));
        Vector3 trailEnd = emergencyVehicle.position;
        DrawLine3D(trailStart, trailEnd, COLOR_TRAIL);
    }
}
