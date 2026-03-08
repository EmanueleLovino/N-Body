#pragma once
#include <concepts>
#include <iterator>
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
concept particles_container = std::ranges::contiguous_range<C>;

// (TODO) add a concept for System (like AoS particles and SoA particles
// themselves) (TODO) add a concept for Integrator as well

template <typename V>
concept is_particle_view = requires(V v) {
    v.qx;
    v.qy;
    v.qz;
    v.vx;
    v.vy;
    v.vz;
    v.ax;
    v.ay;
    v.az;
    v.m;
};
template <typename S>
concept particles_system = requires(S s) {
    { s.begin() };
    { s.end() };
    { s.size() } -> std::convertible_to<std::size_t>;
} && requires(S s) {
    { *s.begin() } -> is_particle_view;
};
