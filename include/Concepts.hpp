#pragma once
#include <concepts>
#include <cstddef>
#include <type_traits>

template <typename T>
concept static_array_t = std::is_trivially_copyable_v<T>;

template <typename T>
concept Scalar = std::is_arithmetic_v<T>;

// still unused, we keep it for now
template <typename S, typename T>
concept StoragePolicy = requires(S s, std::size_t i) {
  { s.qx(i) } -> std::same_as<T &>;
  { s.qy(i) } -> std::same_as<T &>;
  { s.qz(i) } -> std::same_as<T &>;
  { s.vx(i) } -> std::same_as<T &>;
  { s.vy(i) } -> std::same_as<T &>;
  { s.vz(i) } -> std::same_as<T &>;
  { s.ax(i) } -> std::same_as<T &>;
  { s.ay(i) } -> std::same_as<T &>;
  { s.az(i) } -> std::same_as<T &>;
  { s.m(i) } -> std::same_as<T &>;
  { s.r(i) } -> std::same_as<T &>;
};
