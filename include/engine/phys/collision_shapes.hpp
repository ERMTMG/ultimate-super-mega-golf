#pragma once

#include "../vec2.hpp"
#include "variant_wrapper.hpp"

namespace prim::phys {

struct CircleShape {
    float radius;
};

struct RectShape {
    Vec2 size;
};

using Shape = VariantWrapper<CircleShape, RectShape>;

}
