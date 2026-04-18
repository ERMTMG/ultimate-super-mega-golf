#pragma once 

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

}