#include "engine/phys/rigid_body.hpp"
#include "engine/math.hpp"
#include "engine/phys/collision_shapes.hpp"
#include "engine/phys/units.hpp"
#include "engine/vec2.hpp"
#include <numbers>

namespace prim::phys {

Body::Body(
    const Vec2Meters& position,
    const Vec2MetersPerSec& linear_velocity,
    float rotation,
    RadiansPerSec angular_velocity,
    Kilograms mass,
    float restitution,
    KilogramsPerMeterSq density,
    MetersSq area,
    bool is_static,
    Shape&& shape
) noexcept : m_position(position), m_linear_velocity(linear_velocity), m_rotation(rotation), m_angular_velocity(angular_velocity),
    m_properties { .mass = mass, .restitution = restitution, .density = density, .area = area },
    m_static(is_static), m_shape(shape) {}

Body Body::circle(Meters radius, const Vec2Meters &position, bool is_static, Kilograms mass, float restitution) noexcept {
    MetersSq circle_area = std::numbers::pi_v<float> * radius * radius;
    if(Body::CheckRestitutionBounds) {
        clamp_inplace(restitution, Body::MinRestitution, Body::MaxRestitution);
    }
    return Body(
        position,
        Vec2::Zero,
        0.f,
        0.f,
        mass,
        restitution,
        mass / circle_area, // density = mass / area, in 2D at least
        circle_area,
        is_static,
        { CircleShape { radius } }
    );
}

Body Body::circle_with_density(Meters radius, const Vec2Meters &position, bool is_static, GramsPerCentimeterSq density, float restitution) noexcept {
    MetersSq circle_area = std::numbers::pi_v<float> * radius * radius;
    KilogramsPerMeterSq std_density = to_kg_per_m2(density);
    if(Body::CheckRestitutionBounds) {
        clamp_inplace(restitution, Body::MinRestitution, Body::MaxRestitution);
    }
    return Body(
        position,
        Vec2::Zero,
        0.f,
        0.f,
        density * circle_area, // mass = density * area
        restitution,
        std_density,
        circle_area,
        is_static,
        { CircleShape { radius } }
    );
}

Body Body::rectangle(const Vec2Meters &size, const Vec2Meters &position, bool is_static, Kilograms mass, float restitution) noexcept {
    MetersSq rectangle_area = size.x * size.y;
    if(Body::CheckRestitutionBounds) {
        clamp_inplace(restitution, Body::MinRestitution, Body::MaxRestitution);
    }
    return Body(
        position,
        Vec2::Zero,
        0.f,
        0.f,
        mass,
        restitution,
        mass / rectangle_area, // density = mass / area, in 2D at least
        rectangle_area,
        is_static,
        { RectShape { size } }
    );
}

Body Body::rectangle_with_density(const Vec2Meters& size, const Vec2Meters &position, bool is_static, GramsPerCentimeterSq density, float restitution) noexcept {
    MetersSq rectangle_area = size.x * size.y;
    if(Body::CheckRestitutionBounds) {
        clamp_inplace(restitution, Body::MinRestitution, Body::MaxRestitution);
    }
    KilogramsPerMeterSq std_density = to_kg_per_m2(density);
    return Body(
        position,
        Vec2::Zero,
        0.f,
        0.f,
        density * rectangle_area, // mass = density * area
        restitution,
        std_density,
        rectangle_area,
        is_static,
        { RectShape { size } }
    );
}

const Shape& Body::shape() const noexcept { return this->m_shape; }

const Vec2Meters& Body::position() const noexcept { return this->m_position; }

void Body::set_position(const Vec2Meters& new_position) noexcept {
    this->m_position = new_position;
}

void Body::move_by(const Vec2Meters& increment) noexcept {
    this->m_position += increment;
}

float Body::rotation() const noexcept { return this->m_rotation; }

void Body::set_rotation(float new_rotation) noexcept { 
    this->m_rotation = new_rotation;
}

void Body::rotate_by(float increment) noexcept {
    this->m_rotation += increment;
}

}