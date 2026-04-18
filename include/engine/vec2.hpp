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
    
    // Defaulting copy/move constructors/assignments

    inline Vec2(const Vec2&) noexcept = default;
    inline Vec2(Vec2&&) noexcept = default;
    inline Vec2& operator=(const Vec2&) noexcept = default;
    inline Vec2& operator=(Vec2&&) noexcept = default;

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

    // Special case, because saying `2 * v` is sometimes nicer than `v * 2`.
    friend Vec2 operator*(float scalar, const Vec2& v) noexcept;

    inline Vec2 operator/(this const Vec2& v, float scalar) noexcept {
        return { v.x / scalar, v.y * scalar };
    }

    // Operator assignments (why aren't these overloaded automatically smh)

    inline Vec2& operator+=(this Vec2& v_dest, const Vec2& src) noexcept {
        v_dest.x += src.x;
        v_dest.y += src.y;
        return v_dest;
    }

    inline Vec2& operator-=(this Vec2& v_dest, const Vec2& src) noexcept {
        v_dest.x -= src.x;
        v_dest.y -= src.y;
        return v_dest;
    }

    inline Vec2& operator*=(this Vec2& v_dest, float scalar) noexcept {
        v_dest.x *= scalar;
        v_dest.y *= scalar;
        return v_dest;
    }

    inline Vec2& operator/=(this Vec2& v_dest, float scalar) noexcept {
        v_dest.x /= scalar;
        v_dest.y /= scalar;
        return v_dest;
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

    inline bool is_unit() const noexcept {
        return prim::approx_eq(this->x * this->x + this->y * this->y, 1.f);
    }

    // Normalizes the vector in-place. Calling this on a vector with length zero is not accounted for.
    inline void normalize() noexcept {
        float len = std::sqrt(this->x * this->x + this->y * this->y);
        this->x /= len;
        this->y /= len;
    }

    // version of `.normalized()` that checks if the vector has norm zero, and does nothing if so. 
    // Returns `true` if the vector was modified (i.e. if it doesn't have length zero)
    inline bool normalize_checked() noexcept {
        float len_squared = this->x * this->x + this->y * this->y;
        if(len_squared != 0.0f && len_squared != -0.0f) {
            float len = std::sqrt(len_squared);
            this->x /= len;
            this->y /= len;
            return true;
        }
        return false;
    }

    // Returns the result of lerping `from` to `to` by factor `t`.
    inline Vec2 lerp(this const Vec2& from, const Vec2& to, float t) noexcept {
        return {
            std::lerp(from.x, to.x, t),
            std::lerp(from.y, to.y, t)
        };
    }

    // Returns the result of lerping `from` to `to` by factors `ts` component-wise.
    // That is, the `x` components are lerped by `ts.x` and similarly for the `y`s.
    // Will this be useful ever?
    inline Vec2 lerp_component_wise(this const Vec2& from, const Vec2& to, const Vec2& ts) noexcept {
        return {
            std::lerp(from.x, to.x, ts.x),
            std::lerp(from.y, to.y, ts.y)
        };
    }

    // Lerps `from` towards `to` by factor `t` inplace, modifying `from`.
    inline void lerp_inplace(this Vec2& from, const Vec2& to, float t) noexcept {
        prim::lerp_inplace(from.x, to.x, t);
        prim::lerp_inplace(from.y, to.y, t);
    }

    // Lerps `from` towards `to` by factors `ts` component-wise and inplace, modifying `from`.
    inline void lerp_inplace_component_wise(this Vec2& from, const Vec2& to, const Vec2& ts) noexcept {
        prim::lerp_inplace(from.x, to.x, ts.x);
        prim::lerp_inplace(from.y, to.y, ts.y);
    }

    // Component-wise or Hadamard product. Useful for scaling on non-square proportions.
    inline Vec2 component_wise_product(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return {
            v_1.x * v_2.x,
            v_1.y * v_2.y
        };
    }

    // Is to `.component_wise_product()` what `operator*=` is to `operator*`.
    inline Vec2& component_wise_multiply(this Vec2& v_1, const Vec2& v_2) noexcept {
        v_1.x *= v_2.x;
        v_1.y *= v_2.y;
        return v_1;
    }

    // Inverse of component-wise product.
    inline Vec2 component_wise_division(this const Vec2& v_1, const Vec2& v_2) noexcept {
        return {
            v_1.x / v_2.x,
            v_1.y / v_2.y
        };
    }

    // Is to `.component_wise_division()` what `operator/=` is to `operator/`.
    inline Vec2& component_wise_divide(this Vec2& v_1, const Vec2& v_2) noexcept {
        v_1.x /= v_2.x;
        v_1.y /= v_2.y;
        return v_1;
    }

    // Converts to Raylib's `Vector2` struct.
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