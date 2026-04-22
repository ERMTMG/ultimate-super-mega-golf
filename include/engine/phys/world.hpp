#pragma once

#include <cstddef>
#include <raylib.h>
#include <vector>
#include "engine/phys/collision_tests.hpp"
#include "engine/phys/units.hpp"
#include "rigid_body.hpp"

namespace prim::phys {

class World {
  private:
    std::vector<Body> m_bodies;
    Vec2MetersPerSecSq m_gravity;
    float m_units_per_meter;
    bool m_gravity_enabled;
    
    void resolve_collision(Body& body_a, Body& body_b, const collision::CollisionRecord& collision) noexcept;

  public:  
    using BodyId = size_t;
    static constexpr size_t DefaultWorldCapacity = 32;
    static constexpr float DefaultUnitsPerMeter = 16.f;
    static inline const Vec2MetersPerSecSq DefaultGravity = { 0.0f, 9.8f };

    World();

    void set_gravity(const Vec2MetersPerSecSq& gravity) noexcept;
    void disable_gravity() noexcept;

    BodyId add_body(Body&& body) noexcept;
    Body& get_body(BodyId id);
    const Body& get_body(BodyId id) const;

    void update(Seconds dt);
    void draw(const ::Camera2D& camera, [[maybe_unused]] Seconds dt = 0.f) const;
};

}