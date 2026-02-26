#pragma once
#include "Concepts.hpp"
#include <cstddef>
#include <span>

namespace nbody {
namespace detail {

/// @brief non-owning view of a Particle
template <Scalar T> struct ParticleView {
    T& qx;
    T& qy;
    T& qz;
    T& vx;
    T& vy;
    T& vz;
    T& ax;
    T& ay;
    T& az;
    T& m;
    T& r;
};

/// @brief forward iterator used in the SoA implementation
template <typename Container> struct Iterator_particles {
    using iterator_category = std::forward_iterator_tag;
    using size_type = std::size_t;

    explicit Iterator_particles(Container* container, size_type index)
        : container_(container), i(index) {};

    /// @brief deferencing operator for the iterator
    /// @requires Container to implement a method that returns the view of a
    /// particle
    /// @ returns a particle view over the i-th element of the container
    auto operator*() { return container_->view(i); }

    Iterator_particles& operator++() {
        ++i;
        return *this;
    }

    bool operator==(const Iterator_particles& other) const {
        return i == other.i;
    }

    bool operator!=(const Iterator_particles& other) const {
        return i != other.i;
    }

  private:
    Container* container_{nullptr};
    size_type i{};
};
} // namespace detail

/// @brief class that organizes particles in a Array of Struct layout
/// @tparam Container: underlying container type, must store elements in a
/// contiguous way in memory (TODO: add a concept)
/// @tparam T: must be a scalar, respecting the Scalar concept

template <template <typename...> class Container, Scalar T = float>
class AoS_particles {
  public:
    using size_type = std::size_t;
    using value_type = T;

    /// Ranges interface
    [[nodiscard]] auto begin() { return data_.begin(); }
    [[nodiscard]] auto end() { return data_.end(); }
    /// size getter, redirects to the underlying container
    [[nodiscard]] size_type size() const { return data_.size(); }

  private:
    /// @brief struct of a single particle
    struct Particle {
        T qx, qy, qz;
        T vx, vy, vz;
        T ax, ay, az;
        T m;
        T r;
    };

    /// @brief underlying container of particles, forming an Array of Struct
    Container<Particle> data_;
};

/// @brief class that organizes particles in a Struct of Array layout
/// @tparam Container: underlying container type, must store elements in a
/// contiguous way in memory (TODO: add a concept)
/// @tparam T: must be a scalar, respecting the Scalar concept

template <template <typename...> class Container, Scalar T = float>
class SoA_particles {
    using iterator = detail::Iterator_particles<SoA_particles>;
    using value_type = T;
    using size_type = std::size_t;

  private:
    Container<T> qx, qy, qz;
    Container<T> vx, vy, vz;
    Container<T> ax, ay, az;
    Container<T> m;
    Container<T> r;

  public:
    /// Ranges interface
    [[nodiscard]] auto begin() { return iterator{this, 0}; }
    [[nodiscard]] auto end() { return iterator{this, qx.size()}; }

    /// view method offering a particleView to the iterator, must have for
    /// std::range support
    [[nodiscard]] detail::ParticleView<T> view(size_type i) {
        return {qx[i], qy[i], qz[i], vx[i], vy[i], vz[i],
                ax[i], ay[i], az[i], m[i],  r[i]};
    }

    /// safe to look at just one dimension as invariants will always hold since
    /// we can only add a full formed particle
    [[nodiscard]] size_type size() { return qx.size(); }
};

} // namespace nbody
