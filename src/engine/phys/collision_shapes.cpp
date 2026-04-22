#include "engine/phys/collision_shapes.hpp"
#include "engine/phys/collision_tests.hpp"
#include "engine/phys/units.hpp"
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

namespace prim::phys {

collision::CollisionRecord get_collision_info(const Shape& shape_a, const Shape& shape_b, const Vec2Meters& offset_a, const Vec2Meters& offset_b) {
    constexpr static auto pair_switch_hash = [](size_t a, size_t b) -> size_t {
        return a * Shape::num_variants() + b;
    };

    switch(pair_switch_hash(shape_a.index(), shape_b.index())) {
      case pair_switch_hash(Shape::index_of<CircleShape>(), Shape::index_of<CircleShape>()): {
        Meters radius_a = shape_a.as<CircleShape>().radius;
        Meters radius_b = shape_b.as<CircleShape>().radius;
        return collision::test_collision_circle_circle(offset_a, radius_a, offset_b, radius_b);
      }  
      case pair_switch_hash(Shape::index_of<CircleShape>(), Shape::index_of<RectShape>()): {
        throw std::logic_error("TODO");
      } 
      case pair_switch_hash(Shape::index_of<RectShape>(), Shape::index_of<CircleShape>()): {
        throw std::logic_error("TODO");
      }
      case pair_switch_hash(Shape::index_of<RectShape>(), Shape::index_of<RectShape>()): {
        throw std::logic_error("TODO");
      }
      default:
        std::abort(); // Debería ser inalcanzable
    }
}

}