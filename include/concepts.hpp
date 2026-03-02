#pragma once
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <type_traits>

template <typename T>
concept static_array_t = std::is_trivially_copyable_v<T>;

template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

/// TODO add a concept for the Container in AoS Particles
struct SoA {};
struct AoS {};

template <typename Tag>
concept is_layout_tag = std::same_as<Tag, AoS> || std::same_as<Tag, SoA>;

/// concept do model a contiguous container, meaning that objects of the
/// container are store physically in a contiguous way, we also need random
/// access support through operator []
template <typename C>
concept Particles_container = std::ranges::contiguous_range<C>;
