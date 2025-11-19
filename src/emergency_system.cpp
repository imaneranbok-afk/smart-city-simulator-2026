#include "emergency_system.h"
#include "raymath.h"
#include <cmath>

// Variables globales
Vehicle emergencyVehicle = { 0 };
Emergency currentEmergency = { 0 };

void InitEmergencySystem(const CityMap& city) {
    emergencyVehicle.position = (Vector3){ 0.0f, 1.0f, 0.0f };
    emergencyVehicle.direction = (Vector3){ 1.0f, 0.0f, 0.0f };
    emergencyVehicle.speed = 0.0f;
    emergencyVehicle.color = RED;
    emergencyVehicle.is_priority = false;
    emergencyVehicle.id = 1;

    currentEmergency.is_active = false;
    currentEmergency.intensity = 0.0f;
    currentEmergency.target_building_index = -1;
}

void StartEmergency(CityMap& city) {
    if (city.buildings.empty()) return;

    // Sélection aléatoire d'un bâtiment
    int targetIndex = rand() % city.buildings.size();

    currentEmergency.location = city.buildings[targetIndex].position;
    currentEmergency.is_active = true;
    currentEmergency.intensity = 1.0f;
    currentEmergency.target_building_index = targetIndex;

    emergencyVehicle.is_priority = true;
    emergencyVehicle.speed = 5.0f;
}

void StopEmergency() {
    currentEmergency.is_active = false;
    currentEmergency.intensity = 0.0f;
    currentEmergency.target_building_index = -1;
    emergencyVehicle.is_priority = false;
    emergencyVehicle.speed = 0.0f;
}

void UpdateEmergencySystem(float deltaTime) {
    if (emergencyVehicle.is_priority && currentEmergency.is_active) {
        Vector3 toTarget = Vector3Subtract(currentEmergency.location, emergencyVehicle.position);
        float distance = Vector3Length(toTarget);

        if (distance > 2.0f) {
            emergencyVehicle.direction = Vector3Normalize(toTarget);
            Vector3 velocity = Vector3Scale(emergencyVehicle.direction, emergencyVehicle.speed * deltaTime);
            emergencyVehicle.position = Vector3Add(emergencyVehicle.position, velocity);
        }
    }
}

void DrawEmergencyEffects(const CityMap& city) {
    if (currentEmergency.is_active && currentEmergency.target_building_index >= 0) {
        const Building& targetBuilding = city.buildings[currentEmergency.target_building_index];
        Vector3 center = { 
            targetBuilding.position.x, 
            targetBuilding.position.y + targetBuilding.size.y / 2.0f, 
            targetBuilding.position.z 
        };

        float pulse = (sin(GetTime() * 5.0f) + 1.0f) / 2.0f;
        Color highlightColor = ColorAlpha(COLOR_HIGHLIGHT_BUILDING, 0.5f + pulse * 0.5f);

        DrawCube(center, targetBuilding.size.x * 1.1f, targetBuilding.size.y * 1.1f, targetBuilding.size.z * 1.1f, highlightColor);
    }

    if (emergencyVehicle.is_priority) {
        // Véhicule
        Vector3 vehicleSize = { 2.0f, 1.5f, 4.0f };
        Vector3 vehicleCenter = { 
            emergencyVehicle.position.x, 
            emergencyVehicle.position.y + vehicleSize.y / 2.0f, 
            emergencyVehicle.position.z 
        };
        DrawCube(vehicleCenter, vehicleSize.x, vehicleSize.y, vehicleSize.z, emergencyVehicle.color);
        DrawCubeWires(vehicleCenter, vehicleSize.x, vehicleSize.y, vehicleSize.z, BLACK);

        // Sirène
        float sirenTime = GetTime() * 10.0f;
        Color sirenColor = (fmod(sirenTime, 2.0f) < 1.0f) ? COLOR_SIREN_RED : COLOR_SIREN_BLUE;
        
        Vector3 lightPos = Vector3Add(vehicleCenter, (Vector3){ 0.0f, vehicleSize.y / 2.0f + 0.5f, 0.0f });
        DrawSphere(lightPos, 0.3f, sirenColor);
    }
}