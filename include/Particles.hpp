#pragma once
#include "Concepts.hpp"
#include <cstddef>
#include <span>

namespace nbody {
/// @brief struct of a single particle
template <Scalar T> struct Particle {
    T qx, qy, qz;
    T vx, vy, vz;
    T ax, ay, az;
    T m;
    T r;
};

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

    using size_type = std::size_t;
    using value_type = T;

  private:
    /// @brief underlying container of particles, forming an Array of Struct
    Container<Particle<T>> data_;

  public:
    /// @brief API method to add a full particle, taken by value. Redirects on
    /// the Container push_back fn.
    /// @requires a Particle p
    void add_particle(Particle<T> p) { data_.push_back(std::move(p)); }

    /// @brief simple reserve API that redirects to the underlying container
    /// @params n, number of Particles which must be allocated
    ///
    void reserve(size_type n) { data_.reserve(n); }
    /// Ranges interface
    [[nodiscard]] auto begin() { return data_.begin(); }
    [[nodiscard]] auto end() { return data_.end(); }
    /// size getter, redirects to the underlying container
    [[nodiscard]] size_type size() const { return data_.size(); }
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
    /// @brief method to add a particle, must scatter all the params to the
    /// underlying container
    /// @params Particle struct
    void add_particle(Particle<T> p) {
        qx.push_back(p.qx);
        qy.push_back(p.qy);
        qz.push_back(p.qz);

        vx.push_back(p.vx);
        vy.push_back(p.vy);
        vz.push_back(p.vz);

        ax.push_back(p.ax);
        ay.push_back(p.ay);
        az.push_back(p.az);

        m.push_back(p.m);
        r.push_back(p.r);
    }

    /// @brief useful method to reserve up to n elements per Container,
    /// redirecting the request to each
    /// @params n, number of Particles which will be allocated
    void reserve(size_type n) {
        qx.reserve(n);
        qy.reserve(n);
        qz.reserve(n);
        vx.reserve(n);
        vy.reserve(n);
        vz.reserve(n);
        ax.reserve(n);
        ay.reserve(n);
        az.reserve(n);
        m.reserve(n);
        r.reserve(n);
    }

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
