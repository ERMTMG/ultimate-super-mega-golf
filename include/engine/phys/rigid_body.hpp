#pragma once

#include "collision_shapes.hpp"
#include "engine/phys/collision_tests.hpp"
#include "units.hpp"

namespace prim::phys {

class Body {
  private:
    Vec2Meters m_position; // Specifically, the position of the center of mass
    Vec2MetersPerSec m_linear_velocity;
    float m_rotation;
    RadiansPerSec m_angular_velocity;

    const struct {
        const Kilograms mass;
        const float restitution; // Dimensionless
        const KilogramsPerMeterSq density;
        const MetersSq area;
    } m_properties;

    const bool m_static; 

    const Shape m_shape;

    Body(
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
    ) noexcept;
        
  public:
    static constexpr Kilograms DefaultMass = 1.0f;
    static constexpr float DefaultRestitution = 0.5f;
    static constexpr KilogramsPerMeterSq DefaultDensity = 1.0f;
    static constexpr MetersSq DefaultArea = 1.0f;

    static constexpr float MinRestitution = 0.0f;
    static constexpr float MaxRestitution = 1.0f;
    static inline bool CheckRestitutionBounds = false;

    // Area is determined by radius, density is determined by mass.
    static Body circle(Meters radius, const Vec2Meters& position, bool is_static, Kilograms mass = DefaultMass, float restitution = DefaultRestitution) noexcept;

    // Makes mass be determined by density instead of vice versa.
    static Body circle_with_density(Meters radius, const Vec2Meters& position, bool is_static, GramsPerCentimeterSq density, float restitution = DefaultRestitution) noexcept;

    static Body rectangle(const Vec2Meters& size, const Vec2Meters& position, bool is_static, Kilograms mass = DefaultMass, float restitution = DefaultRestitution) noexcept;

    static Body rectangle_with_density(const Vec2Meters& size, const Vec2Meters& position, bool is_static, GramsPerCentimeterSq density, float restitution = DefaultRestitution) noexcept;


    const Shape& shape() const noexcept;

    const Vec2Meters& position() const noexcept;
    void set_position(const Vec2Meters& new_position) noexcept;
    // Increments the position instead of outright setting it.
    void move_by(const Vec2Meters& increment) noexcept;

    const Vec2MetersPerSec& linear_velocity() const noexcept;
    // Sets velocity to zero
    void stop() noexcept;
    void add_velocity(const Vec2MetersPerSec& increment) noexcept;

    float rotation() const noexcept;
    void set_rotation(float new_rotation) noexcept; 
    // Increments the rotation instead of outright setting it.
    void rotate_by(float increment) noexcept;

    Kilograms mass() const noexcept;
    bool is_static() const noexcept;
    // Returns the inverse of the mass for non-static bodies and zero for static ones, since static bodies are considered to have infinite mass.
    InvKilograms effective_inv_mass() const noexcept;

    void apply_impulse(Vec2NewtonSeconds impulse) noexcept;

    Vec2Meters global_to_local_pos(const Vec2Meters& global_pos) const noexcept;
    Vec2Meters local_to_global_pos(const Vec2Meters& local_pos) const noexcept;

    void update(Seconds dt);

    collision::CollisionRecord get_collision_with(const Body& other) const noexcept;
};



}