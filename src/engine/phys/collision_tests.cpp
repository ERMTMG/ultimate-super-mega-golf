#include "engine/phys/collision_tests.hpp"
#include "engine/vec2.hpp"

namespace prim::phys::collision {

CollisionRecord CollisionRecord::no_collision() noexcept {
    return { 
        .normal = Vec2::Zero, 
        .relative_contact_pos_a = Vec2::Zero,
        .relative_contact_pos_b = Vec2::Zero,
        .collision_depth = 0.f, 
        .hit = false 
    };
}

CollisionRecord test_collision_circle_circle(const Vec2& center_a, float radius_a, const Vec2& center_b, float radius_b) noexcept {
    float radii_sum = radius_a + radius_b;
    Vec2 center_sep = center_b - center_a;
    if(center_sep.len_squared() <= radii_sum * radii_sum) {
        auto [normal, real_sep] = center_sep.dir_mag(); 
        Vec2 contact_pos_a = normal * radius_a;
        Vec2 contact_pos_b = -normal * radius_b;
        return {
            .normal = normal,
            .relative_contact_pos_a = contact_pos_a,
            .relative_contact_pos_b = contact_pos_b,
            .collision_depth = radii_sum - real_sep,
            .hit = true
        };
    } else {
        return CollisionRecord::no_collision();
    }
}

}