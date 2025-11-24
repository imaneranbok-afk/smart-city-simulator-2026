#include "city_renderer.h"
#include "raymath.h"
#include "emergency_system.h" 
#include <cmath>

// --- Constantes de Rendu ---
const float ROAD_RENDER_HEIGHT = 0.5f; 
// RÉDUCTION DU MULTIPLICATEUR pour accompagner la réduction de la hauteur maximale des bâtiments dans city_generator.cpp
const float BUILDING_HEIGHT_MULTIPLIER = 1.0f; // Anciennement 3.0f, réduit à 1.0f pour des bâtiments plus trapus
const float GLOW_INTENSITY_FACTOR = 0.5f;

// Fonction utilitaire pour dessiner les marquages de route (Lignes néon pulsées)
void DrawRoadMarkings(const RoadSegment& r) {
    if (!r.hasGlowingMarkings) return;

    Vector3 start = r.start;
    Vector3 end = r.end;
    Color color = r.markingColor;

    start.y = ROAD_RENDER_HEIGHT + 0.01f;
    end.y = ROAD_RENDER_HEIGHT + 0.01f;

    Vector3 direction = Vector3Normalize(Vector3Subtract(end, start));
    float length = Vector3Distance(start, end);

    float segmentLength = 5.0f;
    float gapLength = 8.0f;
    float totalMarkingLength = segmentLength + gapLength;

    float pulse = (sin(GetTime() * 4.0f) + 1.0f) / 2.0f; // Effet pulsé
    Color glowColor = ColorAlpha(color, 200 + (int)(pulse * 55)); // De 200 à 255

    for (float currentPos = 0.0f; currentPos < length; currentPos += totalMarkingLength) {
        if (currentPos + segmentLength > length) segmentLength = length - currentPos;
        if (segmentLength <= 0.0f) break;

        Vector3 currentStart = Vector3Add(start, Vector3Scale(direction, currentPos));
        Vector3 currentEnd = Vector3Add(currentStart, Vector3Scale(direction, segmentLength));

        // Calculer le centre et la taille du segment
        Vector3 segmentCenter = Vector3Add(currentStart, Vector3Scale(direction, segmentLength / 2.0f));
        float segmentWidth = 0.5f; // Largeur de la ligne
        
        // S'assurer que les marquages sont plats
        segmentCenter.y = ROAD_RENDER_HEIGHT + 0.01f; 

        if (fabs(direction.x) > fabs(direction.z)) { // Horizontal (direction Z)
            DrawCube(segmentCenter, segmentLength, 0.1f, segmentWidth, glowColor);
        } else { // Vertical (direction X)
            DrawCube(segmentCenter, segmentWidth, 0.1f, segmentLength, glowColor);
        }
    }
}

// Fonction pour dessiner les fenêtres et accents lumineux d'un bâtiment
void DrawBuildingAccents(const Building& b) {
    if (!b.hasGlowingWindows) return;

    Vector3 center = { b.position.x, b.position.y + b.size.y * BUILDING_HEIGHT_MULTIPLIER / 2.0f, b.position.z };
    float height = b.size.y * BUILDING_HEIGHT_MULTIPLIER;
    float width = b.size.x;
    float depth = b.size.z;
    float glow = b.glowIntensity * GLOW_INTENSITY_FACTOR;
    Color accent = ColorAlpha(b.accentColor, (int)(255 * glow));

    float windowSize = 2.0f;
    float spacing = 1.0f;
    float windowHeight = 1.5f;
    float windowSpacingV = 2.0f;

    // Dessin des fenêtres/lignes lumineuses
    // Faces X (avant/arrière)
    for (float xOffset = -width / 2.0f + spacing; xOffset < width / 2.0f - spacing; xOffset += windowSize + spacing) {
        for (float yOffset = windowSpacingV / 2.0f; yOffset < height - windowSpacingV / 2.0f; yOffset += windowHeight + windowSpacingV) {
            Vector3 posFront = { center.x + xOffset, b.position.y + yOffset, center.z - depth / 2.0f + 0.01f };
            Vector3 posBack = { center.x + xOffset, b.position.y + yOffset, center.z + depth / 2.0f - 0.01f };
            
            DrawCube(posFront, windowSize, windowHeight, 0.01f, accent);
            DrawCube(posBack, windowSize, windowHeight, 0.01f, accent);
        }
    }

    // Faces Z (côtés)
    for (float zOffset = -depth / 2.0f + spacing; zOffset < depth / 2.0f - spacing; zOffset += windowSize + spacing) {
         for (float yOffset = windowSpacingV / 2.0f; yOffset < height - windowSpacingV / 2.0f; yOffset += windowHeight + windowSpacingV) {
            Vector3 posLeft = { center.x - width / 2.0f + 0.01f, b.position.y + yOffset, center.z + zOffset };
            Vector3 posRight = { center.x + width / 2.0f - 0.01f, b.position.y + yOffset, center.z + zOffset };
            
            DrawCube(posLeft, 0.01f, windowHeight, windowSize, accent);
            DrawCube(posRight, 0.01f, windowHeight, windowSize, accent);
         }
    }

    // Ajout d'une antenne ou d'un détail sur le toit
    if (b.hasHelipad) {
        Vector3 helipadCenter = { center.x, b.position.y + height + 0.5f, center.z };
        DrawCylinder(helipadCenter, 0.5f, 0.5f, 1.0f, 16, b.accentColor);
        DrawCircle3D(helipadCenter, 10.0f, (Vector3){1, 0, 0}, 90.0f, ColorAlpha(b.accentColor, 100)); // H sur le toit
    }
}


// Fonction pour dessiner un seul bâtiment
void DrawBuilding(const Building& b) {
    // Calculer la taille et la position centrale ajustées par le multiplicateur
    float rendered_height = b.size.y * BUILDING_HEIGHT_MULTIPLIER;
    Vector3 center = { b.position.x, b.position.y + rendered_height / 2.0f, b.position.z };

    // Couleur principale
    Color mainColor = b.mainColor;

    // Dessiner le cube principal (structure du bâtiment)
    DrawCube(center, b.size.x, rendered_height, b.size.z, mainColor);
    DrawCubeWires(center, b.size.x, rendered_height, b.size.z, BLACK);

    // Dessiner les détails lumineux (fenêtres, accents)
    DrawBuildingAccents(b);
}

// Fonction pour dessiner un segment de route
void DrawRoadSegment(const RoadSegment& r) {
    float width = r.width;
    float road_length = Vector3Distance(r.start, r.end);
    Vector3 road_center = Vector3Lerp(r.start, r.end, 0.5f);
    
    // Pour une route plane (sol), la hauteur y sera la moitié de l'épaisseur de rendu
    road_center.y = ROAD_RENDER_HEIGHT / 2.0f;

    bool is_horizontal = fabs(r.start.z - r.end.z) < 0.01f && fabs(r.start.x - r.end.x) > 0.01f;
    bool is_vertical = fabs(r.start.x - r.end.x) < 0.01f && fabs(r.start.z - r.end.z) > 0.01f;

    // Couleur de la route, assombrie pour l'effet nuit
    Color darkRoadColor = { r.roadColor.r / 3, r.roadColor.g / 3, r.roadColor.b / 3, 255 };

    if (is_horizontal) { 
        DrawCube(road_center, road_length, ROAD_RENDER_HEIGHT, width, darkRoadColor);
        DrawRoadMarkings(r);
    } 
    else if (is_vertical) {
        DrawCube(road_center, width, ROAD_RENDER_HEIGHT, road_length, darkRoadColor);
        DrawRoadMarkings(r);
    } 
    // Intersection
    else if (road_length < width * 2) {
         DrawCube(road_center, width, ROAD_RENDER_HEIGHT, width, darkRoadColor);
    }
}

// Fonction pour dessiner les feux de signalisation holographiques
void DrawTrafficLight(const TrafficLight& t) {
    // Base du feu (poteau)
    DrawCylinder((Vector3){t.position.x, 5.0f, t.position.z}, 0.5f, 0.5f, 10.0f, 8, DARKGRAY);

    // Position des lumières holographiques
    Vector3 lightPos = {t.position.x, 15.0f, t.position.z};

    // Calcul de la couleur lumineuse
    Color lightColor = BLANK;
    if (t.isRed) lightColor = RED;
    else if (t.isYellow) lightColor = YELLOW;
    else if (t.isGreen) lightColor = GREEN;

    if (lightColor.a != 0) {
        // Dessiner le "bulbe" du feu
        DrawSphere(lightPos, 2.0f, ColorAlpha(lightColor, 200));

        // Dessiner l'effet de halo (pulse)
        float pulse = (sin(GetTime() * 8.0f) + 1.0f) / 2.0f; // Vitesse de pulsation
        float haloRadius = 2.0f + pulse * 1.0f;
        DrawSphere(lightPos, haloRadius, ColorAlpha(lightColor, 80));
    }
}

// Fonction pour dessiner les passages piétons lumineux
void DrawCrosswalk(const Crosswalk& c) {
    if (!c.isActive) return;

    Vector3 center = { c.position.x, ROAD_RENDER_HEIGHT + 0.01f, c.position.z };
    float pulse = (sin(GetTime() * 3.0f) + 1.0f) / 2.0f;
    Color glowColor = ColorAlpha(COLOR_CROSSWALK_NEON, 150 + (int)(pulse * 105)); // 150 à 255

    // Dessiner la zone du passage piéton comme un rectangle lumineux
    DrawCube(center, c.width, 0.05f, c.length, glowColor);

    // Dessiner les lignes
    float stripeWidth = 1.5f;
    float stripeGap = 1.0f;
    int numStripes = (int)(c.length / (stripeWidth + stripeGap));
    
    for (int i = 0; i < numStripes; ++i) {
        Vector3 stripeCenter = center;
        stripeCenter.z -= c.length / 2.0f - (stripeWidth / 2.0f) - i * (stripeWidth + stripeGap);
        DrawCube(stripeCenter, c.width * 0.9f, 0.1f, stripeWidth, ColorAlpha(WHITE, 200));
    }
}

// Fonction principale de rendu de la carte
void DrawCityMap(const CityMap& city) {
    
    // Dessiner le sol de base (Planète/Fond très sombre)
    DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 5000.0f, 5000.0f }, COLOR_GROUND_GRID); // Utilise la constante pour le sol

    // 1. Rendu des routes
    for (const auto& r : city.roads) {
        DrawRoadSegment(r);
    }
    
    // 2. Rendu des bâtiments
    for (const auto& b : city.buildings) {
        DrawBuilding(b);
    }

    // 3. Rendu des feux tricolores et passages piétons (qui peuvent être liés aux routes)
    for (const auto& t : city.trafficLights) {
        DrawTrafficLight(t);
    }
     for (const auto& c : city.crosswalks) {
        DrawCrosswalk(c);
    }

    // Dessiner l'effet d'urgence (véhicule et surbrillance du bâtiment)
    DrawEmergencyEffects(city);
}