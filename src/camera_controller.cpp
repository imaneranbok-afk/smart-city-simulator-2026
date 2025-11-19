#include "camera_controller.h"
#include "raymath.h"

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
    // Rotation
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 mouseDelta = GetMouseDelta();
        float yaw = -mouseDelta.x * CAMERA_ROTATION_SPEED;

        Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
        Matrix rotationYaw = MatrixRotate(camera->up, yaw);
        forward = Vector3Transform(forward, rotationYaw);
        camera->target = Vector3Add(camera->position, forward);
    }

    // Mouvement
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera->up));
    Vector3 moveVector = { 0.0f, 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) moveVector = Vector3Add(moveVector, forward);
    if (IsKeyDown(KEY_S)) moveVector = Vector3Subtract(moveVector, forward);
    if (IsKeyDown(KEY_A)) moveVector = Vector3Subtract(moveVector, right);
    if (IsKeyDown(KEY_D)) moveVector = Vector3Add(moveVector, right);

    moveVector.y = 0.0f;
    moveVector = Vector3Normalize(moveVector);
    moveVector = Vector3Scale(moveVector, CAMERA_MOVE_SPEED);

    camera->position = Vector3Add(camera->position, moveVector);
    camera->target = Vector3Add(camera->target, moveVector);

    // Monter/Descendre
    if (IsKeyDown(KEY_Q)) camera->position.y -= CAMERA_MOVE_SPEED;
    if (IsKeyDown(KEY_E)) camera->position.y += CAMERA_MOVE_SPEED;
    camera->target.y = camera->position.y;

    // Zoom
    float wheelMove = GetMouseWheelMove();
    camera->fovy -= wheelMove * CAMERA_ZOOM_SPEED;
    if (camera->fovy < 10.0f) camera->fovy = 10.0f;
    if (camera->fovy > 120.0f) camera->fovy = 120.0f;
}

void UpdateFollowCamera(Camera3D* camera, const Vector3& targetPosition) {
    Vector3 offset = { -15.0f, 20.0f, -15.0f };
    camera->position = Vector3Add(targetPosition, offset);
    camera->target = targetPosition;
}

void UpdateCameraMode(Camera3D* camera, const Vector3& followTarget) {
    if (IsKeyPressed(KEY_ONE)) currentCameraMode = CUSTOM_CAMERA_MODE_FREE;
    if (IsKeyPressed(KEY_TWO)) currentCameraMode = CUSTOM_CAMERA_MODE_FOLLOW;
    if (IsKeyPressed(KEY_THREE)) {
        currentCameraMode = CUSTOM_CAMERA_MODE_STRATEGIC;
        camera->position = (Vector3){ 0.0f, 150.0f, 0.0f };
        camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
        camera->up = (Vector3){ 0.0f, 0.0f, -1.0f };
    }

    switch (currentCameraMode) {
        case CUSTOM_CAMERA_MODE_FREE:
            UpdateFreeCamera(camera);
            break;
        case CUSTOM_CAMERA_MODE_FOLLOW:
            UpdateFollowCamera(camera, followTarget);
            break;
        case CUSTOM_CAMERA_MODE_STRATEGIC:
            // Vue stratégique fixe
            break;
    }
}