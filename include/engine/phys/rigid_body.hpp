#pragma once

#include "collision_shapes.hpp"
#include "units.hpp"

namespace prim::phys {

class Body {
  private:
    Vec2Meters m_position;
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
    float rotation() const noexcept;
    void set_rotation(float new_rotation) noexcept; 
};



}