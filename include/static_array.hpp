#include "Concepts.hpp"
#include <cstddef>
#include <initializer_list>
#include <ranges>

namespace nbody {

template <static_array_t T, std::size_t N> class static_array {

  using value_type = T;
  using size_type = std::size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;

public:
  // public API
  constexpr reference operator[](size_type i) noexcept { return elems_[i]; }
  constexpr const_reference operator[](size_type i) const noexcept {
    return elems_[i];
  }

  constexpr pointer begin() noexcept { return elems_; }
  constexpr const_pointer begin() const noexcept { return elems_; }

  constexpr pointer end() noexcept { return elems_ + N; }
  constexpr const_pointer end() const noexcept { return elems_ + N; }

  constexpr pointer data() noexcept { return elems_; }
  constexpr const_pointer data() const noexcept { return elems_; }

  constexpr size_type size() const noexcept { return N; }

private:
  alignas(T) T elems_[N];
};

/*
 * In order for nbody::static_array to behave as an aggregate, we can't define
 * inside any constructor. We insert here an helper function for testing
 * purposes
 */
template <static_array_t T, std::size_t N>
constexpr static_array<T, N> make_static_array(std::initializer_list<T> list) {
  if (list.size() != N) {
    throw std::length_error("make_static_array: wrong number of elements");
  }
  static_array<T, N> a{};
  std::ranges::copy(list, a.begin());
  return a;
}

}; // namespace nbody
