#include <raylib.h>
#include "imgui/imgui.h"
#include "rlImGui.h"


int main(int, char**) {
    InitWindow(800, 600, "Hello World!");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(
                "Hello from Raylib!", 
                400 - MeasureText("Hello from Raylib!", 20)/2, 
                290, 
                20, BLACK
            );
            rlImGuiBegin();
                ImGui::Text("Hello from ImGui!");
            rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}