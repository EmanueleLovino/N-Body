#pragma once
#include <cstddef>
#include <iterator>

#include "detail/particle_view.hpp"

namespace nbody::detail {
/// @brief forward iterator used in the SoA implementation
template <typename Storage>
struct Iterator_particles {
    /// all aliases required for iterator compatibility with STL
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = ParticleView<typename Storage::value_type>;
    using pointer = void;
    using reference = value_type;
    using size_type = std::size_t;

    explicit Iterator_particles(Storage* storage, size_type index)
        : storage_(storage), i(index) {};

    /// @brief deferencing operator for the iterator
    /// @requires Container to implement a method that returns the view of a
    /// particle
    /// @ returns a particle view over the i-th element of the container
    auto operator*() { return storage_->view(i); }

    Iterator_particles& operator++() noexcept {
        ++i;
        return *this;
    }

    bool operator==(const Iterator_particles& other) const noexcept {
        return i == other.i;
    }

    bool operator!=(const Iterator_particles& other) const noexcept {
        return i != other.i;
    }

   private:
    Storage* storage_{nullptr};
    size_type i{};
};
}  // namespace nbody::detail
