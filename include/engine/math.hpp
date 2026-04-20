#pragma once 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <random>

namespace prim {
    
template<typename FloatT>
inline bool approx_eq(FloatT x_1, FloatT x_2, FloatT tolerance) noexcept {
    return (std::abs(x_1 - x_2) < tolerance);
}

template<typename FloatT>
inline bool approx_eq(FloatT x_1, FloatT x_2) noexcept {
    FloatT tolerance = std::abs(x_1 - x_2) * FloatT{1e-8};
    return approx_eq<FloatT>(x_1, x_2, tolerance);
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

namespace random {

    static thread_local inline std::default_random_engine p_engine;

    inline void set_seed(uint64_t seed) noexcept {
        p_engine.seed(seed);
    }

    inline float rand_f() noexcept {
        static thread_local std::uniform_real_distribution<float> s_distribution {0.f, 1.f};
        return s_distribution(p_engine);
    }

    inline float rand_f_range(float from, float to) noexcept {
        return rand_f() * (to - from) + from;
    }

    inline double rand_d() noexcept {
        static thread_local std::uniform_real_distribution<double> s_distribution {0.f, 1.f};
        return s_distribution(p_engine);
    }

    inline double rand_d_range(double from, double to) noexcept {
        return rand_d() * (to - from) + from;
    }

    inline uint64_t rand_i() noexcept {
        return p_engine();
    }

    inline constexpr uint64_t rand_max_val() noexcept {
        return decltype(p_engine)::max();
    }

    // Returns a random number in the range `[0, n)`, guaranteed to be equal chance between `{0, 1,..., n-1}`.
    // May run the RNG more than once, especially for large `n`. 
    inline uint64_t rand_i_mod(uint64_t n) noexcept {
        uint64_t max = p_engine.max();
        uint64_t max_fair = max - (max % n); // Max random value that doesn't introduce cutoff bias 
        uint64_t result;
        do {
            result = p_engine();
        } while(result >= max_fair);
        return (result % n);
    }
}

}