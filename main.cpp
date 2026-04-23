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
    world.disable_gravity();


    world.add_body(Body::circle(100.f, Vec2Meters { 0.f, 110.f }, true, std::numeric_limits<Kilograms>::infinity()));
    world.add_body(Body::circle(100.f, Vec2Meters { 110.f, 0.f }, true, std::numeric_limits<Kilograms>::infinity()));
    world.add_body(Body::circle(100.f, Vec2Meters { 0.f, -110.f }, true, std::numeric_limits<Kilograms>::infinity()));
    world.add_body(Body::circle(100.f, Vec2Meters { -110.f, 0.f }, true, std::numeric_limits<Kilograms>::infinity()));

    world.add_body(Body::circle(0.5, Vec2Meters { 0.f, 0.f }, false, 1.0));
    world.add_body(Body::circle(0.5, Vec2Meters { -0.5f, 0.66f }, false, 1.0));
    world.add_body(Body::circle(0.5, Vec2Meters { -0.5f, -0.66f }, false, 1.0));
    world.add_body(Body::circle(0.5, Vec2Meters { -1.0f, 1.33f }, false, 1.0));
    world.add_body(Body::circle(0.5, Vec2Meters { -1.0f, -1.33f }, false, 1.0));
    world.add_body(Body::circle(0.5, Vec2Meters { -1.0f, 0.f }, false, 1.0));
    world.add_body(Body::circle(0.5, Vec2Meters { -2.0f, 0.f }, false, 1.0));

    auto main_ball_id = world.add_body(Body::circle(0.66, Vec2Meters { 5.f, 0.f }, false, 10.0));
    
    while(!WindowShouldClose()) {
        Seconds dt = GetFrameTime();
        world.update(dt);
        if(IsKeyPressed(KEY_A)) {
            world.get_body(main_ball_id).apply_impulse(Vec2NewtonSeconds { -30.f, 0.f });
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            world.draw(camera, dt);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}