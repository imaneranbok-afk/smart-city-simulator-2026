#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"
#include "raymath.h"

// Constantes de la caméra
const float CAMERA_MOVE_SPEED = 1.0f;
const float CAMERA_ROTATION_SPEED = 0.005f;
const float CAMERA_ZOOM_SPEED = 1.0f;

// Modes de caméra
typedef enum {
    CUSTOM_CAMERA_MODE_FREE = 0,
    CUSTOM_CAMERA_MODE_FOLLOW,
    CUSTOM_CAMERA_MODE_STRATEGIC
} CustomCameraMode;

// Variable globale
extern CustomCameraMode currentCameraMode;

// Fonctions
Camera3D createBaseCamera();
void UpdateFreeCamera(Camera3D* camera);
void UpdateFollowCamera(Camera3D* camera, const Vector3& targetPosition);
void UpdateCameraMode(Camera3D* camera, const Vector3& followTarget);

#endif