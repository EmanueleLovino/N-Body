#pragma once
#include "Concepts.hpp"
#include "Particles.hpp"

namespace nbody {

template <template <typename...> class Container, Scalar T, Layout_tag Layout>
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
using System = Storage<Container, T, Layout>::type;

} // namespace nbody
