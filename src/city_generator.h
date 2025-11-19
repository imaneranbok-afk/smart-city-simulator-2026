#ifndef CITY_GENERATOR_H
#define CITY_GENERATOR_H

#include "city_data.h"

// Fonctions de génération pour les trois layouts
CityMap GenerateFuturisticCity(int size, CityLayout layout);
CityMap GenerateGridLayout(int grid_size, float cell_size);
CityMap GenerateCircularLayout(int rings, float ring_spacing);
CityMap GenerateOrganicLayout(int building_count, float area_size);

// Fonctions pour éléments futuristes
void GenerateFuturisticRoadMarkings(CityMap& city);
void GenerateHolographicTrafficLights(CityMap& city);
void GenerateGlowingCrosswalks(CityMap& city);
void AddFuturisticEffects(CityMap& city);

// Fonctions utilitaires
float GetRandomFloat(float min, float max);
BuildingType GetRandomFuturisticBuildingType();
Color GetFuturisticBuildingColor(BuildingType type);
Color GetFuturisticAccentColor(BuildingType type);

#endif