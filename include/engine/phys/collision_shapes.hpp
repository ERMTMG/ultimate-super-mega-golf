#pragma once

#include "engine/phys/units.hpp"
#include "engine/vec2.hpp"
#include "engine/phys/collision_tests.hpp"
#include "variant_wrapper.hpp"

namespace prim::phys {

struct CircleShape {
    float radius;
};

struct RectShape {
    Vec2 size;
};

using Shape = VariantWrapper<CircleShape, RectShape>;

collision::CollisionRecord get_collision_info(const Shape& shape_a, const Shape& shape_b, const Vec2Meters& offset_a, const Vec2Meters& offset_b);

}
