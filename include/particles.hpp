#pragma once
#include "concepts.hpp"
#include "detail/iterator_particles.hpp"
#include "detail/particle_view.hpp"
#include <cstddef>

namespace nbody {
/// @brief struct of a single particle
template <Scalar T> struct Particle {
    T qx, qy, qz;
    T vx, vy, vz;
    T ax, ay, az;
    T m;
    T r;
};

/// @brief class that organizes particles in a Array of Struct layout
/// @tparam Container: underlying container type, must store elements in a
/// contiguous way in memory
/// @tparam T: must be a scalar, respecting the Scalar concept

template <template <typename...> class Container, Scalar T = float>
    requires Particles_container<Container<Particle<T>>>
class AoS_particles {

  private:
    /// @brief underlying container of particles, forming an Array of Struct
    Container<Particle<T>> data_;

  public:
    using size_type = std::size_t;
    using value_type = T;

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
/// contiguous way in memory
/// @tparam T: must be a scalar, respecting the Scalar concept

template <template <typename...> class Container, Scalar T = float>
    requires Particles_container<Container<Particle<T>>>
class SoA_particles {

  private:
    Container<T> qx, qy, qz;
    Container<T> vx, vy, vz;
    Container<T> ax, ay, az;
    Container<T> m;
    Container<T> r;

  public:
    using iterator = detail::Iterator_particles<SoA_particles>;
    using value_type = T;
    using size_type = std::size_t;

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

/// Type alias with implementing a small compile time dipatching through tags to
/// have a better readability and usage

template <template <typename...> class Container, Scalar T, typename Layout>
struct Storage;

template <template <typename...> class Container, Scalar T>
struct Storage<Container, T, AoS> {
    using type = nbody::AoS_particles<Container, T>;
};

template <template <typename...> class Container, Scalar T>
struct Storage<Container, T, SoA> {
    using type = nbody::SoA_particles<Container, T>;
};

template <template <typename...> class Container, Scalar T, typename Layout>
    requires Particles_container<Container<Particle<T>>>
using System = Storage<Container, T, Layout>::type;

} // namespace nbody
