#pragma once
#include <concepts>
#include <ranges>
#include <type_traits>

template <typename T>
concept static_array_t = std::is_trivially_copyable_v<T>;

template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

struct SoA {};
struct AoS {};

template <typename Tag>
concept is_layout_tag = std::same_as<Tag, AoS> || std::same_as<Tag, SoA>;

/// concept do model a contiguous container, meaning that objects of the
/// container are store physically in a contiguous way, we also need random
template <typename C>
concept Particles_container = std::ranges::contiguous_range<C>;

// (TODO) add a concept for System (like AoS particles and SoA particles
// themselves) (TODO) add a concept for Integrator as well
