#pragma once
#include <cmath>
#include <functional>
#include <limits>
#include <raylib.h>
#include <sys/cdefs.h>

#include "math.hpp"

namespace prim {

struct Vec2 {
    float x;
    float y;

    // Constructors //

    // Basic constructor
    inline Vec2(float x, float y) noexcept : x(x), y(y) {};
    
    // Converts from Raylib's Vector2
    inline static Vec2 from(Vector2 v) noexcept { return { v.x, v.y }; }

    // Converts from polar coordinates where `r` is the radius and `theta` is the angle.
    inline static Vec2 from_polar(float r, float theta) noexcept {
        return { r * std::cos(theta), r * std::sin(theta) };
    }

    // Makes a unit vector pointing ini a specific direction given by `theta`. Equivalent to `Vec2::from_polar(1.0, theta)`.
    inline static Vec2 from_angle(float theta) noexcept {
        return { std::cos(theta), std::sin(theta) };
    }

    // Constants //
    static const Vec2 Zero;
    static const Vec2 One;
    static const Vec2 Left;
    static const Vec2 Right;
    static const Vec2 Up;
    static const Vec2 Down;
    static const Vec2 NaN;
    static const Vec2 Infinity;

    // Operator overloads for arithmetic //

    inline Vec2 operator-(this const Vec2& v) noexcept {
        return Vec2 { -v.x, -v.y };
    }

    inline Vec2 operator+(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return { v_1.x + v_2.x, v_1.y + v_2.y };
    }

    inline Vec2 operator-(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return { v_1.x - v_2.x, v_1.y - v_2.y };
    }

    inline Vec2 operator*(this const Vec2& v, float scalar) noexcept {
        return { v.x * scalar, v.y * scalar };
    }

    friend Vec2 operator*(float scalar, const Vec2& v) noexcept;

    inline Vec2 operator/(this const Vec2& v, float scalar) noexcept {
        return { v.x / scalar, v.y * scalar };
    }

    // Non-operator math functions //

    // Square of the magnitude. Faster to calculate if you only need to, e.g., compare length.
    inline float len_squared() const noexcept {
        return (this->x * this->x) + (this->y * this->y);
    }

    // AKA magnitude, norm,...
    inline float len() const noexcept { 
        return std::sqrt((this->x * this->x) + (this->y * this->y));
    }

    // Dot product
    inline float dot(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return (v_1.x * v_2.x) + (v_1.y * v_2.y);
    }

    // Dubbed 2D cross product sometimes, because it's equal to the length of `(v_1.x, v_1.y, 0) × (v_2.x, v_2.y, 0)`.
    // Also the determinant of the 2D matrix whose columns are `v_1` and `v_2`. Useful fore some calculations.
    inline float cross_2d(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return (v_1.x * v_2.y) - (v_1.y * v_2.x);
    }

    inline float angle_to(const Vec2& other) const noexcept {
        return std::atan2(this->y, this->x) - std::atan2(other.y, other.x);
    }

    // Use this instead of `.angle_to(Vec2::Right)`, just in case.
    inline float angle() const noexcept {
        return std::atan2(this->y, this->x);
    }

    // Comparison operators // 

    // Susceptible to floating point precision errors. Recommended to use `.approx_eq` instead.
    inline bool operator==(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return (v_1.x == v_2.x && v_1.y == v_2.y);
    }

    inline bool operator!=(this const Vec2& v_1, const Vec2 v_2) noexcept {
        return (v_1.x != v_2.x || v_1.y != v_2.y);
    }

    // Uses lexicographical ordering.
    inline bool operator<(this const Vec2& v_1, const Vec2& v_2) noexcept {
        if(v_1.x == v_2.x) {
            return (v_1.y < v_2.y);
        }
        return (v_1.x < v_2.x);
    }

    // Uses lexicographical ordering.
    inline bool operator<=(this const Vec2& v_1, const Vec2& v_2) noexcept {
        if(v_1.x == v_2.x) {
            return (v_1.y <= v_2.y);
        }
        return (v_1.x < v_2.x);
    }

    // Uses lexicographical ordering.
    inline bool operator>(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return v_2 < v_1;
    }

    // Uses lexicographical ordering.
    inline bool operator>=(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return v_2 <= v_1;
    }

    inline bool approx_eq(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return prim::approx_eq(v_1.x, v_2.x) && prim::approx_eq(v_1.y, v_2.y);
    }

    // Converts to Raylib `Vector2` struct.
    inline ::Vector2 to_rl() const noexcept {
        return ::Vector2 { this->x, this->y };
    }
};

inline const Vec2 Vec2::Zero  = { 0.f,  0.f };
inline const Vec2 Vec2::One   = { 1.f,  1.f };
inline const Vec2 Vec2::Left  = { -1.f, 0.f };
inline const Vec2 Vec2::Right = { 1.f,  0.f };
inline const Vec2 Vec2::Up    = { 0.f,  -1.f };
inline const Vec2 Vec2::Down  = { 0.f,  1.f };
 
inline const Vec2 Vec2::Infinity = { std::numeric_limits<float>::infinity(),  std::numeric_limits<float>::infinity() };
inline const Vec2 Vec2::NaN      = { std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() };

inline Vec2 operator*(float scalar, const Vec2& v) noexcept { return v * scalar; }

}

namespace std {

template<>
struct hash<prim::Vec2> {
    inline std::size_t operator()(const prim::Vec2& v) const noexcept {
        auto float_hash = std::hash<float>{};
        return float_hash(v.x) ^ (float_hash(v.y) << 1);
    }
};

}