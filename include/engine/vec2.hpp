#pragma once

#include <cmath>
#include <functional>
#include <limits>
#include <raylib.h>
#include <sys/cdefs.h>
#include <utility>

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

    inline Vec2 operator-() const noexcept {
        return Vec2 { -this->x, -this->y };
    }

    inline Vec2 operator+(const Vec2& rhs) const noexcept {
        return { this->x + rhs.x, this->y + rhs.y };
    }

    inline Vec2 operator-(const Vec2& rhs) const noexcept {
        return { this->x - rhs.x, this->y - rhs.y };
    }

    inline Vec2 operator*(float scalar) const noexcept {
        return { this->x * scalar, this->y * scalar };
    }

    // Special case, because saying `2 * v` is sometimes nicer than `v * 2`.
    friend Vec2 operator*(float scalar, const Vec2& v) noexcept;

    inline Vec2 operator/(float scalar) const noexcept {
        return { this->x / scalar, this->y * scalar };
    }

    // Operator assignments (why aren't these overloaded automatically smh)

    inline Vec2& operator+=(const Vec2& src) noexcept {
        this->x += src.x;
        this->y += src.y;
        return *this;
    }

    inline Vec2& operator-=(const Vec2& src) noexcept {
        this->x -= src.x;
        this->y -= src.y;
        return *this;
    }

    inline Vec2& operator*=(float scalar) noexcept {
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }

    inline Vec2& operator/=(float scalar) noexcept {
        this->x /= scalar;
        this->y /= scalar;
        return *this;
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
    inline float dot(const Vec2& rhs) const noexcept {
        return (this->x * rhs.x) + (this->y * rhs.y);
    }

    // Dubbed 2D cross product sometimes, because it's equal to the length of `(v_1.x, v_1.y, 0) × (v_2.x, v_2.y, 0)`.
    // Also the determinant of the 2D matrix whose columns are `v_1` and `v_2`. Useful fore some calculations.
    inline float cross_2d(const Vec2& rhs) const noexcept {
        return (this->x * rhs.y) - (this->y * rhs.x);
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
    inline bool operator==(const Vec2& rhs) const noexcept {
        return (this->x == rhs.x && this->y == rhs.y);
    }

    inline bool operator!=(const Vec2 rhs) const noexcept {
        return (this->x != rhs.x || this->y != rhs.y);
    }

    // Uses lexicographical ordering.
    inline bool operator<(const Vec2& rhs) const noexcept {
        if(this->x == rhs.x) {
            return (this->y < rhs.y);
        }
        return (this->x < rhs.x);
    }

    // Uses lexicographical ordering.
    inline bool operator<=(const Vec2& rhs) const noexcept {
        if(this->x == rhs.x) {
            return (this->y <= rhs.y);
        }
        return (this->x < rhs.x);
    }

    // Uses lexicographical ordering.
    inline bool operator>(const Vec2& rhs) const noexcept {
        return rhs < *this;
    }

    // Uses lexicographical ordering.
    inline bool operator>=(const Vec2& rhs) const noexcept {
        return rhs <= *this;
    }

    inline bool approx_eq(const Vec2& rhs) noexcept {
        return prim::approx_eq(this->x, rhs.x) && prim::approx_eq(this->y, rhs.y);
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

    // version of `.normalize()` that checks if the vector has norm zero, and does nothing if so. 
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

    // Returns a vector with the same direction as `*this` and length one, without modifying the vector. Calling this on a vector with length zero is not accounted for.
    inline Vec2 normalized() const noexcept {
        float len = std::sqrt(this->x * this->x + this->y * this->y);
        return { this->x / len, this->y / len };
    }

    // Version of `.normalized()` that checks if the vector has norm zero, and returns `Vec2::Zero` if so.
    inline Vec2 normalized_checked() const noexcept {
        float len_squared = this->x * this->x + this->y * this->y;
        if(len_squared != 0.0f && len_squared != -0.0f) {
            float len = std::sqrt(len_squared);
            return { this->x / len, this->y / len };
        }
        return Vec2::Zero;
    }

    // Decomposes the vector into separate direction (unit vector) and magnitude, collected in an `std::pair`.
    // Faster than calling `.normalized()` and `.len()` separately.
    // Returns `{ Vec2::Zero, 0.f }` on a vector with length zero.
    inline std::pair<Vec2, float> dir_mag() const noexcept {
        float len_squared = this->x * this->x + this->y * this->y;
        if(len_squared != 0.0f && len_squared != -0.0f) {
            float len = std::sqrt(len_squared);
            return { Vec2 { this->x / len, this->y / len }, len };
        }
        return { Vec2::Zero, 0.f };
    }

    // Returns the result of lerping `from` to `to` by factor `t`.
    inline Vec2 lerp(const Vec2& to, float t) noexcept {
        return {
            std::lerp(this->x, to.x, t),
            std::lerp(this->y, to.y, t)
        };
    }

    // Returns the result of lerping `from` to `to` by factors `ts` component-wise.
    // That is, the `x` components are lerped by `ts.x` and similarly for the `y`s.
    // Will this be useful ever?
    inline Vec2 lerp_component_wise(const Vec2& to, const Vec2& ts) noexcept {
        return {
            std::lerp(this->x, to.x, ts.x),
            std::lerp(this->y, to.y, ts.y)
        };
    }

    // Lerps `from` towards `to` by factor `t` inplace, modifying `from`.
    inline void lerp_inplace(const Vec2& to, float t) noexcept {
        prim::lerp_inplace(this->x, to.x, t);
        prim::lerp_inplace(this->y, to.y, t);
    }

    // Lerps `from` towards `to` by factors `ts` component-wise and inplace, modifying `from`.
    inline void lerp_inplace_component_wise(const Vec2& to, const Vec2& ts) noexcept {
        prim::lerp_inplace(this->x, to.x, ts.x);
        prim::lerp_inplace(this->y, to.y, ts.y);
    }

    // Component-wise or Hadamard product. Useful for scaling on non-square proportions.
    inline Vec2 component_wise_product(const Vec2& rhs) noexcept {
        return {
            this->x * rhs.x,
            this->y * rhs.y
        };
    }

    // Is to `.component_wise_product()` what `operator*=` is to `operator*`.
    inline Vec2& component_wise_multiply(const Vec2& rhs) noexcept {
        this->x *= rhs.x;
        this->y *= rhs.y;
        return *this;
    }

    // Inverse of component-wise product.
    inline Vec2 component_wise_division(const Vec2& rhs) noexcept {
        return {
            this->x / rhs.x,
            this->y / rhs.y
        };
    }

    // Is to `.component_wise_division()` what `operator/=` is to `operator/`.
    inline Vec2& component_wise_divide(const Vec2& rhs) noexcept {
        this->x /= rhs.x;
        this->y /= rhs.y;
        return *this;
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