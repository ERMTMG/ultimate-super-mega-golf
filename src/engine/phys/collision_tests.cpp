#include "engine/phys/collision_tests.hpp"
#include "engine/vec2.hpp"

namespace prim::phys::collision {

CollisionRecord CollisionRecord::no_collision() noexcept {
    return { 
        .normal = Vec2::Zero, 
        .collision_depth = 0.f, 
        .hit = false 
    };
}

CollisionRecord test_collision_circle_circle(const Vec2& center_1, float radius_1, const Vec2& center_2, float radius_2) noexcept {
    float radii_sum = radius_1 + radius_2;
    Vec2 center_sep = center_2 - center_1;
    if(center_sep.len_squared() <= radii_sum * radii_sum) {
        auto [normal, real_sep] = center_sep.dir_mag(); 
        return {
            .normal = normal,
            .collision_depth = radii_sum - real_sep,
            .hit = true
        };
    } else {
        return CollisionRecord::no_collision();
    }
}

}