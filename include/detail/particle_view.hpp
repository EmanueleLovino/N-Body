#pragma once
#include "concepts.hpp"

namespace nbody::detail {

/// @brief non-owning view of a Particle
template <Scalar T>
struct ParticleView {
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
}  // namespace nbody::detail
