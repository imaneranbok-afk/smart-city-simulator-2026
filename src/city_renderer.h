#ifndef CITY_RENDERER_H
#define CITY_RENDERER_H

#include "raylib.h"
#include "city_data.h"

void DrawBuilding(const Building& b);
void DrawRoadSegment(const RoadSegment& r);
void DrawCityMap(const CityMap& city);

#endif