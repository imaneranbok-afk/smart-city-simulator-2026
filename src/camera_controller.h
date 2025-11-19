#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"
#include "raymath.h"
#include "city_data.h"

// Déclaration de la fonction définie dans main.cpp
Camera3D createBaseCamera();

// Constantes de la caméra
const float CAMERA_MOVE_SPEED = 1.0f;
const float CAMERA_ROTATION_SPEED = 0.005f;
const float CAMERA_ZOOM_SPEED = 1.0f;

// Modes de caméra
typedef enum {
    CUSTOM_CAMERA_MODE_FREE = 0,
    CUSTOM_CAMERA_MODE_FOLLOW,
    CUSTOM_CAMERA_MODE_STRATEGIC,
    CUSTOM_CAMERA_MODE_CINEMATIC // Non implémenté pour l'instant
} CustomCameraMode;

// Variable globale pour le mode de caméra actuel
extern CustomCameraMode currentCameraMode;

// Fonction pour mettre à jour la caméra libre avec les contrôles personnalisés
// Déclaration de la fonction de mise à jour de la caméra de suivi
void UpdateFollowCamera(Camera3D* camera, const Vector3& targetPosition);

// Déclaration de la fonction de mise à jour de la caméra de suivi
void UpdateFollowCamera(Camera3D* camera, const Vector3& targetPosition);

// Déclaration de la fonction principale de mise à jour de la caméra
void UpdateCameraMode(Camera3D* camera, const Vector3& followTarget);

#endif // CAMERA_CONTROLLER_H
