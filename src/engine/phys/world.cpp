#include "engine/phys/world.hpp"
#include "engine/phys/collision_shapes.hpp"
#include "engine/phys/rigid_body.hpp"
#include "engine/phys/units.hpp"
#include "engine/vec2.hpp"
#include <raylib.h>
#include <utility>
#include <vector>

namespace prim::phys {

World::World() : m_bodies{}, m_gravity{DefaultGravity}, m_units_per_meter{World::DefaultUnitsPerMeter}, m_gravity_enabled(true) {
    m_bodies.reserve(World::DefaultWorldCapacity);
}

World::BodyId World::add_body(Body&& body) noexcept {
    auto new_body_id = m_bodies.size();
    m_bodies.emplace_back(std::move(body));
    return new_body_id;
}

Body& World::get_body(World::BodyId id) {
    return m_bodies.at(id);
}

void World::update(Seconds dt) {
    if(m_gravity_enabled) {
        for(auto& body : m_bodies) {
            Kilograms mass = body.mass();
            Vec2NewtonSeconds gravity_impulse = mass * m_gravity * dt;
            body.apply_impulse(gravity_impulse);
        }
    }

    for(auto& body : m_bodies) {
        body.update(dt);
    }
}

void World::draw(const ::Camera2D& camera, Seconds /* unused */) const {
    static constexpr ::Color DefaultDrawColor = ::ORANGE;
    static constexpr float RadToDeg = 180.f / std::numbers::pi;

    BeginMode2D(camera);
        for(const auto& body : m_bodies) {
            const auto& shape = body.shape();
            switch(shape.index()) {
              case Shape::index_of<CircleShape>(): {
                const auto& circle_shape = shape.as<CircleShape>();
                auto [x, y] = body.position() * m_units_per_meter;
                float radius = circle_shape.radius * m_units_per_meter;
                ::DrawCircle((int)x, (int)y, radius, DefaultDrawColor);
                ::DrawCircle(x, y, 2, ::BLACK);
              } break;
              case Shape::index_of<RectShape>(): {
                const auto& rect = shape.as<RectShape>();
                auto [rect_width, rect_height] = rect.size * m_units_per_meter;
                auto [center_x, center_y] = body.position() * m_units_per_meter;
                ::Rectangle drawn_rect = { 
                    center_x,
                    center_y,
                    rect_width, 
                    rect_height 
                };
                ::Vector2 origin = { 
                    rect_width / 2.f,
                    rect_height / 2.f
                };
                ::DrawRectanglePro(drawn_rect, origin, body.rotation() * RadToDeg, DefaultDrawColor); 
                ::DrawCircle(center_x, center_y, 2, ::BLACK);
              } break;
            }
        }
    EndMode2D();
}

}