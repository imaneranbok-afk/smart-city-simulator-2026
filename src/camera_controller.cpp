#include "camera_controller.h"
#include "raymath.h"

// Constantes de la caméra, definies UNE SEULE FOIS dans ce fichier .cpp
const float CAMERA_MOVE_SPEED = 1.0f;
const float CAMERA_ROTATION_SPEED = 0.005f;
const float CAMERA_ZOOM_SPEED = 1.0f;
const float STRATEGIC_PAN_SPEED = 100.0f; // Augmenté pour un déplacement fluide dans une grande ville

// Variable globale
CustomCameraMode currentCameraMode = CUSTOM_CAMERA_MODE_FREE;

Camera3D createBaseCamera() {
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 100.0f, 120.0f, 100.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    return camera;
}

void UpdateFreeCamera(Camera3D* camera) {
    // Rotation (Cliquer et glisser avec le bouton droit de la souris)
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouseDelta = GetMouseDelta();
        float yaw = -mouseDelta.x * CAMERA_ROTATION_SPEED;
        
        // Mettre à jour la direction (yaw)
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
        Matrix rotationYaw = MatrixRotate(camera->up, yaw);
        forward = Vector3Transform(forward, rotationYaw);
        
        // Pitch (Correction : inverser le delta Y pour un contrôle standard)
        float pitch = -mouseDelta.y * CAMERA_ROTATION_SPEED; 
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));
        Matrix rotationPitch = MatrixRotate(right, pitch);

        Vector3 newForward = Vector3Transform(forward, rotationPitch);
        
        // Vérifier si le pitch est trop vertical (pour éviter le basculement de la caméra)
        float dotProduct = Vector3DotProduct(newForward, camera->up);
        if (fabs(dotProduct) < 0.95f) { // Limite pour empêcher le basculement
            forward = newForward;
            camera->target = Vector3Add(camera->position, forward);
        }
    }

    // Mouvement avec les touches (WASD)
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));
    Vector3 moveVector = { 0.0f, 0.0f, 0.0f };

    // Déplacement avant/arrière
    if (IsKeyDown(KEY_W)) moveVector = Vector3Add(moveVector, forward);
    if (IsKeyDown(KEY_S)) moveVector = Vector3Subtract(moveVector, forward);

    // Déplacement latéral (strafe)
    if (IsKeyDown(KEY_A)) moveVector = Vector3Subtract(moveVector, right);
    if (IsKeyDown(KEY_D)) moveVector = Vector3Add(moveVector, right);

    // Normaliser et appliquer la vitesse (si l'on bouge)
    if (Vector3Length(moveVector) > 0.0f) {
        moveVector = Vector3Scale(Vector3Normalize(moveVector), CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f);
        camera->position = Vector3Add(camera->position, moveVector);
        camera->target = Vector3Add(camera->target, moveVector);
    }

    // Monter/Descendre (Q/E)
    if (IsKeyDown(KEY_Q)) {
        camera->position.y -= CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f;
        camera->target.y -= CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f; 
    }
    if (IsKeyDown(KEY_E)) {
        camera->position.y += CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f;
        camera->target.y += CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f; 
    }
}

void UpdateFollowCamera(Camera3D* camera, const Vector3& targetPosition) {
    // Caméra derrière et légèrement au-dessus de la cible
    Vector3 offset = { -15.0f, 20.0f, -15.0f }; 
    camera->position = Vector3Add(targetPosition, offset);
    camera->target = targetPosition;
}

void UpdateStrategicCamera(Camera3D* camera, float deltaTime) {
    // 1. Détermination des vecteurs de direction basés sur la vue
    
    // Le vecteur 'forward' de la caméra stratégique est toujours dirigé vers le bas (axe Y négatif)
    // Mais pour le PANNING (déplacement sur la carte), nous nous intéressons à la direction 
    // "dans le plan XZ" de la caméra.

    // Panning avant/arrière : sur l'axe Z global
    Vector3 forwardPan = (Vector3){ 0.0f, 0.0f, 1.0f };
    // Panning gauche/droite : sur l'axe X global
    Vector3 rightPan = (Vector3){ 1.0f, 0.0f, 0.0f };

    Vector3 panVector = { 0.0f, 0.0f, 0.0f };
    float speed = STRATEGIC_PAN_SPEED * deltaTime;

    // Déplacement avec les touches (W/Z/S/A/D)
    // W/Z : Déplace la vue vers le "haut" de l'écran (diminue Z)
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_Z)) {
        panVector.z -= 1.0f;
    }
    // S : Déplace la vue vers le "bas" de l'écran (augmente Z)
    if (IsKeyDown(KEY_S)) {
        panVector.z += 1.0f;
    }
    // A/Q : Déplace la vue vers la "gauche" de l'écran (diminue X)
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_Q)) {
        panVector.x -= 1.0f;
    }
    // D : Déplace la vue vers la "droite" de l'écran (augmente X)
    if (IsKeyDown(KEY_D)) {
        panVector.x += 1.0f;
    }

    // Appliquer le mouvement de Panning
    if (Vector3Length(panVector) > 0.0f) {
        Vector3 normalizedPan = Vector3Normalize(panVector);
        Vector3 move = Vector3Scale(normalizedPan, speed);

        // Mettre à jour la position de la caméra et sa cible
        camera->position = Vector3Add(camera->position, move);
        camera->target = Vector3Add(camera->target, move);
    } 

    // 2. Zoom (Molette de la souris) - Ajuster la hauteur de la caméra
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0.0f) {
        // Le zoom ajuste la distance entre la position et la cible
        float zoomFactor = wheelMove * 10.0f; 
        
        // Mettre à jour la hauteur (Y) de la caméra
        camera->position.y -= zoomFactor;

        // Limites de hauteur (Zoom)
         if (camera->position.y < 5.0f) camera->position.y = 5.0f;
         if (camera->position.y > 500.0f) camera->position.y = 500.0f; // Permettre d'aller très haut
    }
    
    // S'assurer que la cible reste au niveau du sol (pour la vue Top-Down)
    camera->target.y = 0.0f;
}


void UpdateCameraMode(Camera3D* camera, const Vector3& followTarget) {
    // Changement de mode
    if (IsKeyPressed(KEY_ONE)) currentCameraMode = CUSTOM_CAMERA_MODE_FREE;
    if (IsKeyPressed(KEY_TWO)) currentCameraMode = CUSTOM_CAMERA_MODE_FOLLOW;
    if (IsKeyPressed(KEY_THREE)) {
        // Initialisation du mode stratégique pour une vue parfaitement Top-Down
        if (currentCameraMode != CUSTOM_CAMERA_MODE_STRATEGIC) {
            currentCameraMode = CUSTOM_CAMERA_MODE_STRATEGIC;
            // Position de vue Top-Down : très haut sur Y, regarde le centre (0, 0, 0)
            camera->position = (Vector3){ 0.0f, 150.0f, 0.0f };
            camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
            camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };
            camera->fovy = 60.0f;
        }
    }

    float deltaTime = GetFrameTime();

    // Mise à jour du mode actuel
    switch (currentCameraMode) {
        case CUSTOM_CAMERA_MODE_FREE:
            UpdateFreeCamera(camera);
            break;
        case CUSTOM_CAMERA_MODE_FOLLOW:
            UpdateFollowCamera(camera, followTarget);
            break;
        case CUSTOM_CAMERA_MODE_STRATEGIC:
            UpdateStrategicCamera(camera, deltaTime);
            break;
        default:
            break;
    }
}