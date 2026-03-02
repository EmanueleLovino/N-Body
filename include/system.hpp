#pragma once
#include "concepts.hpp"
#include "particles.hpp"

namespace nbody {

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
