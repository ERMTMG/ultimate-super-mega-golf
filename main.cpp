#include <raylib.h>
#include "engine/phys/rigid_body.hpp"
#include "engine/phys/world.hpp"
#include "engine/vec2.hpp"
//#include "imgui/imgui.h"
#include "rlImGui.h"


int main(int, char**) {
    using prim::Vec2;

    InitWindow(800, 600, "Hello World!");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    ::Camera2D camera { .offset = { 400.f, 300.f }, .target = { 0.f, 0.f }, .rotation = 0.f, .zoom = 1.f };
    prim::phys::World world;
    world.add_body(prim::phys::Body::circle(1.f, Vec2::Zero, true));
    auto rect_id = world.add_body(prim::phys::Body::rectangle(Vec2 { 2.f, 1.5 }, Vec2 { 3.0, 0.0 }, true));

    world.get_body(rect_id).set_rotation(std::numbers::pi / 4);

    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            world.draw(camera);

            world.get_body(rect_id).set_rotation(
                world.get_body(rect_id).rotation() + 0.01f
            );

            world.get_body(rect_id).set_position(
                world.get_body(rect_id).position() + Vec2 { 0.0f, 0.02f }
            );
            //rlImGuiBegin();
            //    ImGui::Text("Hello from ImGui!");
            //rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}