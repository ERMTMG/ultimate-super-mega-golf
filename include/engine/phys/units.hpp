#pragma once

#include "engine/vec2.hpp"
namespace prim::phys {

using Seconds = float; // Unit of time

using Meters = float; // Unit of length and distance
using MetersPerSec = float; // Unit of velocity

using Vec2Meters = Vec2; // Unit of length and distance (Vec2)
using Vec2MetersPerSec = Vec2; // Unit of velocity (Vec2)
using Vec2MetersPerSecSq = Vec2; // Unit of acceleration (Vec2)

using RadiansPerSec = float; // Unit of angular velocity

using Kilograms = float; // Unit of mass
using InvKilograms = float;
using MetersSq = float; // Unit of area

using KilogramsPerMeterSq = float; // Unit of density
using GramsPerCentimeterSq = float; // Slightly more intuitive unit of density

inline GramsPerCentimeterSq to_g_per_cm2(KilogramsPerMeterSq d) {
    // d kg/m² * (1000g / 1kg) * (1 m² / 10⁴ cm²) = d/10 g/cm²
    static constexpr float MULTIPLIER = 0.1f;
    return d * MULTIPLIER;
}

inline KilogramsPerMeterSq to_kg_per_m2(GramsPerCentimeterSq d) {
    // Inverse of multiplier in function above
    static constexpr float MULTIPLIER = 10.f;
    return d * MULTIPLIER;
}

using Vec2NewtonSeconds = Vec2; // Unit of impulse and momentum (Vec2)

}

