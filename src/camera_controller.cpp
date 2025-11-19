#include "camera_controller.h"
#include "raymath.h"

// Définition de la variable globale
CustomCameraMode currentCameraMode = CUSTOM_CAMERA_MODE_FREE;

// Déclaration de la fonction createBaseCamera (définie dans main.cpp)
extern Camera3D createBaseCamera();

// Fonction pour mettre à jour la caméra libre avec les contrôles personnalisés
void UpdateFreeCamera(Camera3D* camera) {
    // 1. Rotation (Clic droit + Souris)
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouseDelta = GetMouseDelta();
        float yaw = -mouseDelta.x * CAMERA_ROTATION_SPEED;
        float pitch = -mouseDelta.y * CAMERA_ROTATION_SPEED;

        // Calcul du vecteur 'forward' et 'right'
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));

        // Rotation autour de l'axe Y (yaw)
        Matrix rotationYaw = MatrixRotate(camera->up, yaw);
        forward = Vector3Transform(forward, rotationYaw);
        camera->up = Vector3Transform(camera->up, rotationYaw);

        // Rotation autour de l'axe 'right' (pitch)
        Matrix rotationPitch = MatrixRotate(right, pitch);
        forward = Vector3Transform(forward, rotationPitch);
        camera->up = Vector3Transform(camera->up, rotationPitch);

        // Mise à jour de la cible de la caméra
        camera->target = Vector3Add(camera->position, forward);
    }

    // 2. Mouvement horizontal (WASD)
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));
    Vector3 moveVector = { 0.0f, 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) moveVector = Vector3Add(moveVector, forward);
    if (IsKeyDown(KEY_S)) moveVector = Vector3Subtract(moveVector, forward);
    if (IsKeyDown(KEY_A)) moveVector = Vector3Subtract(moveVector, right);
    if (IsKeyDown(KEY_D)) moveVector = Vector3Add(moveVector, right);

    // Ignorer la composante Y pour le mouvement horizontal
    moveVector.y = 0.0f;
    moveVector = Vector3Normalize(moveVector);
    moveVector = Vector3Scale(moveVector, CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f); // Multiplier par 60 pour une vitesse constante

    camera->position = Vector3Add(camera->position, moveVector);
    camera->target = Vector3Add(camera->target, moveVector);

    // 3. Monter/Descendre (Q/E)
    Vector3 verticalMove = { 0.0f, 0.0f, 0.0f };
    if (IsKeyDown(KEY_Q)) verticalMove.y -= CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f;
    if (IsKeyDown(KEY_E)) verticalMove.y += CAMERA_MOVE_SPEED * GetFrameTime() * 60.0f;

    camera->position = Vector3Add(camera->position, verticalMove);
    camera->target = Vector3Add(camera->target, verticalMove);

    // 4. Zoom (Molette) - Affecte le FOV
    float wheelMove = GetMouseWheelMove();
    camera->fovy -= wheelMove * CAMERA_ZOOM_SPEED;
    if (camera->fovy < 1.0f) camera->fovy = 1.0f;
    if (camera->fovy > 120.0f) camera->fovy = 120.0f;

    // 5. Réinitialiser la vue (R)
    if (IsKeyPressed(KEY_R)) {
        *camera = createBaseCamera(); // Nécessite que createBaseCamera soit accessible
    }
}

// Fonction pour mettre à jour la caméra de suivi
void UpdateFollowCamera(Camera3D* camera, const Vector3& targetPosition) {
    // Logique de caméra de suivi simple (vue arrière)
    Vector3 offset = { -10.0f, 10.0f, -10.0f }; // Décalage derrière et au-dessus
    camera->position = Vector3Add(targetPosition, offset);
    camera->target = targetPosition;
    camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };
}

// Fonction principale de mise à jour de la caméra
void UpdateCameraMode(Camera3D* camera, const Vector3& followTarget) {
    // Gestion du changement de mode (touches 1-4)
    if (IsKeyPressed(KEY_ONE)) currentCameraMode = CUSTOM_CAMERA_MODE_FREE;
    if (IsKeyPressed(KEY_TWO)) currentCameraMode = CUSTOM_CAMERA_MODE_FOLLOW;
    if (IsKeyPressed(KEY_THREE)) {
        currentCameraMode = CUSTOM_CAMERA_MODE_STRATEGIC;
        // Positionnement initial pour la vue stratégique (vue de dessus)
        camera->position = (Vector3){ 0.0f, 150.0f, 0.0f };
        camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
        camera->up = (Vector3){ 0.0f, 0.0f, -1.0f }; // Le "haut" est l'axe Z négatif pour une vue de dessus
    }
    // La touche F bascule en mode suivi si elle n'est pas déjà active
    if (IsKeyPressed(KEY_F)) {
        currentCameraMode = CUSTOM_CAMERA_MODE_FOLLOW;
    }

    // Mise à jour en fonction du mode
    switch (currentCameraMode) {
        case CUSTOM_CAMERA_MODE_FREE:
            UpdateFreeCamera(camera);
            break;
        case CUSTOM_CAMERA_MODE_FOLLOW:
            UpdateFollowCamera(camera, followTarget);
            break;
        case CUSTOM_CAMERA_MODE_STRATEGIC:
            // Pour l'instant, pas de mouvement en mode stratégique, juste la vue
            break;
        case CUSTOM_CAMERA_MODE_CINEMATIC:
            // Logique de caméra cinématique (à implémenter)
            break;
    }
}
