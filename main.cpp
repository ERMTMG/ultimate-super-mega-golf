#include <limits>
#include <raylib.h>
#include "engine/phys/rigid_body.hpp"
#include "engine/phys/units.hpp"
#include "engine/phys/world.hpp"
#include "engine/vec2.hpp"
//#include "imgui/imgui.h"
#include "rlImGui.h"


int main(int, char**) {
    using prim::Vec2;
    using namespace prim::phys;

    InitWindow(800, 600, "Hello World!");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    ::Camera2D camera { .offset = { 400.f, 300.f }, .target = { 0.f, 0.f }, .rotation = 0.f, .zoom = 1.f };
    World world;
    world.add_body(Body::circle(20.f, Vec2Meters { 0.f, 20.f }, true, std::numeric_limits<Kilograms>::infinity()));
    world.add_body(Body::circle(0.5f, Vec2Meters { 3.f, -10.f }, false, 1.f));

    while(!WindowShouldClose()) {
        Seconds dt = GetFrameTime();
        world.update(dt);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            world.draw(camera, dt);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}