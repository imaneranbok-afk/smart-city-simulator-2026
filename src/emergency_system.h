#ifndef EMERGENCY_SYSTEM_H
#define EMERGENCY_SYSTEM_H

#include "raylib.h"
#include "city_data.h"
#include "emergency_data.h"

void InitEmergencySystem(const CityMap& city);
void StartEmergency(CityMap& city);
void StopEmergency();
void UpdateEmergencySystem(float deltaTime);
void DrawEmergencyEffects(const CityMap& city);

#endif // EMERGENCY_SYSTEM_H
