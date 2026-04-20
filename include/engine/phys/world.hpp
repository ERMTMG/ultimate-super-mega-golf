#pragma once

#include <cstddef>
#include <raylib.h>
#include <vector>
#include "engine/phys/units.hpp"
#include "rigid_body.hpp"

namespace prim::phys {

class World {
  private:
    std::vector<Body> m_bodies;
    float m_units_per_meter;
  public:  
    using BodyId = size_t;
    static constexpr size_t DefaultWorldCapacity = 32;
    static constexpr float DefaultUnitsPerMeter = 16.f;


    World();

    BodyId add_body(Body&& body) noexcept;
    Body& get_body(BodyId id);
    const Body& get_body(BodyId id) const;

    void update(Seconds dt);
    void draw(const ::Camera2D& camera, [[maybe_unused]] Seconds dt = 0.f) const;
};

}