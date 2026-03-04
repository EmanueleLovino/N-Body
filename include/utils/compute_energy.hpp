#pragma once
#include <cmath>

#include "constants.hpp"

namespace nbody::utils {

/// @brief util function to compute the total energy of a given system
/// @tparams system of particles, either SoA or AoS
/// @return total energy in Joules
/// @note system should be marked const, this would require the implementation
/// of a const iterator and duplication of ranges API
template <typename System>
auto compute_energy(System& system) {
    using T = typename System::value_type;
    constexpr auto G = constants::G;
    T kinetic{0.0};
    T potential{0.0};

    for (auto&& pi : system) {
        auto vel_squared = pi.vx * pi.vx + pi.vy * pi.vy + pi.vz * pi.vz;
        kinetic += 0.5f * pi.m * vel_squared;

        for (auto&& pj : system) {
            if (&pi.qx == &pj.qx) continue;

            auto dx = pi.qx - pj.qx;
            auto dy = pi.qy - pj.qy;
            auto dz = pi.qz - pj.qz;

            auto dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            potential += -(G * pi.m * pj.m) / dist;
        }
    }
    return kinetic + 0.5f * potential;
}
}  // namespace nbody::utils
