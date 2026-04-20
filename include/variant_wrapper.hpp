#pragma once

#include <cstddef>
#include <variant>

template<typename ...Ts>
struct VariantWrapper : std::variant<Ts...> {
  private:
    using BaseType = std::variant<Ts...>;
  public:
    using BaseType::variant;
    using BaseType::operator=;
    using BaseType::emplace;

    template<typename T>
    constexpr bool is() const noexcept {
        constexpr auto occurrences = (std::is_same_v<T, Ts> + ...);
        static_assert(occurrences != 0, "The variant cannot have the type");
        static_assert(occurrences <= 1, "The variant has duplicates of the type");
        return std::holds_alternative<T>(*this);
    }

    template<typename T>
    constexpr const T& as() const noexcept {
        constexpr auto occurrences = (std::is_same_v<T, Ts> + ...);
        static_assert(occurrences != 0, "The variant cannot have the type");
        static_assert(occurrences <= 1, "The variant has duplicates of the type");
        return std::get<T>(*this);
    }

    template<typename T>
    constexpr T& as() noexcept {
        constexpr auto occurrences = (std::is_same_v<T, Ts> + ...);
        static_assert(occurrences != 0, "The variant cannot have the type");
        static_assert(occurrences <= 1, "The variant has duplicates of the type");
        return std::get<T>(*this);
    }

    #if 0 // This is too cursed for me bruh
    template<typename T>
    constexpr T&& move_as() noexcept {
        constexpr auto occurrences = (std::is_same_v<T, Ts> + ...);
        static_assert(occurrences != 0, "The variant cannot have the type");
        static_assert(occurrences <= 1, "The variant has duplicates of the type");
        return std::get<T>(std::move(*static_cast<BaseType*>(this)));
    }
    #endif

    template<typename T>
    constexpr static size_t index_of() noexcept {
        constexpr auto occurrences = (std::is_same_v<T, Ts> + ...);
        static_assert(occurrences != 0, "The variant cannot have the type");
        static_assert(occurrences <= 1, "The variant has duplicates of the type");
        size_t i = 0; 
        ((!std::is_same_v<T, Ts> && ++i) && ...); // Usa la cortocircuitación de && al encontrar `false` 
        return i;
    }
};