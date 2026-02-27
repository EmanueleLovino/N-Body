#pragma once
#include <concepts>
#include <cstddef>
#include <type_traits>

template <typename T>
concept static_array_t = std::is_trivially_copyable_v<T>;

template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

/// TODO add a concept for the Container in AoS Particles
struct SoA {};
struct AoS {};

template <typename Tag>
concept Layout_tag = std::same_as<Tag, AoS> || std::same_as<Tag, SoA>;
