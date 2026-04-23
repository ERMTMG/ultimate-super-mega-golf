#include "engine/phys/rigid_body.hpp"
#include "engine/math.hpp"
#include "engine/phys/collision_shapes.hpp"
#include "engine/phys/collision_tests.hpp"
#include "engine/phys/units.hpp"
#include "engine/vec2.hpp"
#include <cassert>
#include <cmath>
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
  m_static(is_static), m_shape(shape) {
    assert(mass > 0.f);
    assert(density > 0.f);
    assert(area > 0.f);  
}

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

const Vec2MetersPerSec& Body::linear_velocity() const noexcept { return this->m_linear_velocity; }

void Body::stop() noexcept { this->m_linear_velocity = Vec2MetersPerSec::Zero; }

void Body::add_velocity(const Vec2MetersPerSec& increment) noexcept {
    this->m_linear_velocity += increment;
}

float Body::rotation() const noexcept { return this->m_rotation; }

void Body::set_rotation(float new_rotation) noexcept { 
    this->m_rotation = new_rotation;
}

void Body::rotate_by(float increment) noexcept {
    this->m_rotation += increment;
}

Kilograms Body::mass() const noexcept { return m_properties.mass; }

bool Body::is_static() const noexcept { return m_static; }

InvKilograms Body::effective_inv_mass() const noexcept {
    return m_static ? 0.0 : (1.0 / m_properties.mass);
}

void Body::apply_impulse(Vec2NewtonSeconds impulse) noexcept {
    if(this->effective_inv_mass() > 0.0f) {
        this->m_linear_velocity += impulse * effective_inv_mass();
    }
}

Vec2Meters Body::global_to_local_pos(const Vec2Meters& global_pos) const noexcept {
    Vec2Meters temp = global_pos - this->m_position;
    return temp.rotated_by(-m_rotation);
}

Vec2Meters Body::local_to_global_pos(const Vec2Meters& local_pos) const noexcept {
    return local_pos.rotated_by(m_rotation) + this->m_position;
}

void Body::update(Seconds dt) {
    m_position += m_linear_velocity * dt;
}

collision::CollisionRecord Body::get_collision_with(const Body& other) const noexcept {
    const auto& this_shape = this->shape();
    const auto& other_shape = other.shape();

    return get_collision_info(this_shape, other_shape, this->position(), other.position());
}

}