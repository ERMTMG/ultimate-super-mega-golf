#include "engine/phys/world.hpp"
#include "engine/phys/collision_shapes.hpp"
#include "engine/phys/collision_tests.hpp"
#include "engine/phys/rigid_body.hpp"
#include "engine/phys/units.hpp"
#include "engine/vec2.hpp"
#include <cmath>
#include <cstddef>
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

const Body& World::get_body(World::BodyId id) const {
    return m_bodies.at(id);
}

void World::set_gravity(const Vec2MetersPerSecSq& gravity) noexcept {
    m_gravity = gravity;
}
void World::disable_gravity() noexcept {
    m_gravity_enabled = false;
}


void World::resolve_collision(Body& body_a, Body& body_b, const collision::CollisionRecord& collision) noexcept {
    InvKilograms inv_mass_a = body_a.effective_inv_mass();
    InvKilograms inv_mass_b = body_b.effective_inv_mass();

    Vec2MetersPerSec velocity_diff = body_a.linear_velocity() - body_b.linear_velocity();
    NewtonSeconds impulse_quantity = -2.0f * velocity_diff.dot(collision.normal) / (inv_mass_a + inv_mass_b);
    Vec2NewtonSeconds impulse_vec  = impulse_quantity * collision.normal;

    body_b.apply_impulse(-impulse_vec);
    body_a.apply_impulse(impulse_vec);

    float multiplier_to_move_a = inv_mass_a / (inv_mass_a + inv_mass_b);
    float multiplier_to_move_b = inv_mass_b / (inv_mass_a + inv_mass_b);
    Vec2Meters contact_point_a_global = body_a.local_to_global_pos(collision.relative_contact_pos_a);
    Vec2Meters contact_point_b_global = body_b.local_to_global_pos(collision.relative_contact_pos_b);
    Vec2Meters move_direction = contact_point_b_global - contact_point_a_global;
    body_a.move_by(move_direction * multiplier_to_move_a);
    body_b.move_by(-move_direction * multiplier_to_move_b);

    
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

    size_t n = m_bodies.size();
    for(size_t i = 0; i < n; i++) {
        for(size_t j = i + 1; j < n; j++) {
            auto& body_i = m_bodies[i];
            auto& body_j = m_bodies[j];
            if(body_i.effective_inv_mass() == 0.0f && body_j.effective_inv_mass() == 0.0f) {
                continue; // Bodies with infinite mass and static bodies aren't gonna move anyway
            } 
            auto collision = body_i.get_collision_with(body_j);
            if(collision.hit) {
                resolve_collision(body_i, body_j, collision);
            }
        }
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