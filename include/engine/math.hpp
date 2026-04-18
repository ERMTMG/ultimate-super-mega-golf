#pragma once 

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace prim {

template<typename FloatT>
inline bool approx_eq(FloatT x_1, FloatT x_2) noexcept {
    FloatT tolerance = std::abs(x_1 - x_2) * FloatT{1e-8};
    return approx_eq<FloatT>(x_1, x_2, tolerance);
}

template<typename FloatT>
inline bool approx_eq(FloatT x_1, FloatT x_2, FloatT tolerance) noexcept {
    return (std::abs(x_1 - x_2) < tolerance);
}

// Clamps `x` between `min` and `max` in-place, possibly modifying the value. Returns `true` iff `x` was modified.
template<typename FloatT> 
inline bool clamp_inplace(FloatT& x, FloatT min, FloatT max) noexcept {
    FloatT clamped_x = std::clamp(x, min, max);
    if(x == clamped_x) {
        return false;
    } else {
        x = clamped_x;
        return true;
    }
}

template<typename FloatT>
inline void lerp_inplace(FloatT& from, FloatT to, FloatT t) noexcept {
    from += (to - from) * t;
}

}