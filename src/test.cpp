#include "raylib.h"

int main() {
    InitWindow(800, 600, "Test Raylib");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);
        DrawText("RAYLIB FONCTIONNE!", 100, 100, 40, WHITE);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}