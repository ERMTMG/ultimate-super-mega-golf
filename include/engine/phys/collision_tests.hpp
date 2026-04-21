#pragma once

#include "engine/vec2.hpp"
namespace prim::phys::collision {

struct CollisionRecord {
    Vec2 normal;
    float collision_depth;
    bool hit;

    static CollisionRecord no_collision() noexcept;
    
    inline operator bool() const noexcept { return this->hit; };
};

CollisionRecord test_collision_circle_circle(const Vec2& center_1, float radius_1, const Vec2& center_2, float radius_2) noexcept;

}